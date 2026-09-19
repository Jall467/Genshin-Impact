// genshin_radiance.c
// 原神 5.0 后捕获明光抽卡概率计算器
// 编译：gcc -O2 -o genshin_radiance genshin_radiance.c -lm
// 运行：./genshin_radiance

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

/* ======================== 用户配置区 ======================== */

#define NUM_CHAR      2     // 目标UP角色数（满命=7）
#define NUM_GEAR      0     // 目标UP武器数（满精=5）
#define CR_INIT       1     // 捕获明光初始计数器（0~3）

// 捕获明光触发概率表：索引为计数器值 0/1/2/3
// 这是简化模型，官方机制尚未完全公开
static const double CR_P[4] = { 0.0, 0.0, 0.0, 1.0 };

// 是否计算武器池（5.0后定轨命定值1）
#define CALC_GEAR     0

/* ============================================================ */

/* ---------- 常量 ---------- */
#define PITY_CHAR     90    // 角色池5星硬保底
#define PITY_GEAR     80    // 武器池5星硬保底
#define MAX_COUNTER   4     // 捕获明光计数器状态数

template<class T>
int length(T& a) {
    return sizeof(a) / sizeof(a[0]);
}

/* ---------- 5星保底抽数分布 ---------- */
// 角色池：1~73抽 0.6%，74~89抽递增，90抽必出
static void build_char_pity(double* p) {
    for (int i = 1; i <= 73; i++) p[i] = 0.006;
    for (int i = 74; i <= 89; i++) p[i] = 0.006 + 0.06 * (i - 73);
    p[90] = 1.0;
}

// 武器池：1~62抽 0.7%，63~76抽递增，77~80抽必出
static void build_gear_pity(double* p) {
    for (int i = 1; i <= 62; i++) p[i] = 0.007;
    for (int i = 63; i <= 76; i++) p[i] = 0.007 + 0.07 * (i - 62);
    for (int i = 77; i <= 80; i++) p[i] = 1.0;
}

/* ---------- 由单抽概率表算"恰好第n抽出5星"的分布 ---------- */
// ben[n] = P(前n-1抽都没出) * p[n]
static void build_ben(double* p, double* ben, int pity) {
    ben[0] = 0.0;
    double survive = 1.0;
    for (int i = 1; i <= pity; i++) {
        ben[i] = survive * p[i];
        survive *= (1.0 - p[i]);
    }
}

/* ---------- 卷积：a * b -> c，长度 la, lb ---------- */
static void convolve(const double* a, int la, const double* b, int lb, double* c) {
    int lc = la + lb - 1;
    for (int i = 0; i < lc; i++) c[i] = 0.0;
    for (int i = 0; i < la; i++) {
        if (a[i] == 0.0) continue;
        for (int j = 0; j < lb; j++) {
            c[i + j] += a[i] * b[j];
        }
    }
}

/* ============================================================
 *  第一层：状态机层
 *  计算 P_5star[n][m] = 抽 n 个UP 恰好消耗 m 个5星 的概率
 * ============================================================ */

 /*
  * 状态：计数器 c ∈ {0,1,2,3}
  * 从状态 c 出发抽一个UP，有三种路径：
  *   路径1 小保底中UP：概率 0.5 - cr_p[c]/2，消耗 1 个5星，c -> max(c-1,0)
  *   路径2 触发明光：  概率 cr_p[c]，        消耗 1 个5星，c -> 1
  *   路径3 歪+大保底： 概率 0.5 - cr_p[c]/2，消耗 2 个5星，c -> min(c+1,3)
  *
  * 注意：路径1和路径3概率相同，都是 0.5 - cr_p[c]/2
  *       因为小保底只有"中"和"歪"两种结果，明光是从"歪"里额外触发的
  */

  // 返回 P_5star 二维数组，维度 [num_up+1][max_5star+1]
  // max_5star 是理论上限：最坏情况每个UP消耗2个5星，再加初始补偿
static double** compute_5star_dist(int num_up, int cr_init, int* out_max_5star) {
    // 理论上限：最坏情况每个UP歪一次，消耗2个5星
    // 加 2 作为安全余量
    int max_5star = num_up * 2 + 2;
    *out_max_5star = max_5star;

    // dp[n][m][c]：抽 n 个UP，消耗 m 个5星，当前计数器 c
    double*** dp = (double***)calloc(num_up + 1, sizeof(double**));
    for (int n = 0; n <= num_up; n++) {
        dp[n] = (double**)calloc(max_5star + 1, sizeof(double*));
        for (int m = 0; m <= max_5star; m++) {
            dp[n][m] = (double*)calloc(MAX_COUNTER, sizeof(double));
        }
    }
    dp[0][0][cr_init] = 1.0;

    for (int n = 1; n <= num_up; n++) {
        for (int m = 0; m <= max_5star; m++) {
            for (int c = 0; c < MAX_COUNTER; c++) {
                double cur = dp[n - 1][m][c];
                if (cur == 0.0) continue;

                double p_direct = 0.5 - CR_P[c] / 2.0;  // 小保底中UP
                double p_cr = CR_P[c];              // 触发明光
                double p_miss = 0.5 - CR_P[c] / 2.0;  // 歪

                // 路径1：小保底直接中UP，消耗1个5星
                int c1 = (c > 0) ? c - 1 : 0;
                if (p_direct > 0 && m + 1 <= max_5star) {
                    dp[n][m + 1][c1] += cur * p_direct;
                }

                // 路径2：触发明光，消耗1个5星，计数器归1
                if (p_cr > 0 && m + 1 <= max_5star) {
                    dp[n][m + 1][1] += cur * p_cr;
                }

                // 路径3：歪+大保底，消耗2个5星
                int c3 = (c < 3) ? c + 1 : 3;
                if (p_miss > 0 && m + 2 <= max_5star) {
                    dp[n][m + 2][c3] += cur * p_miss;
                }
            }
        }
    }

    // 汇总：对所有计数器求和
    double** P = (double**)calloc(num_up + 1, sizeof(double*));
    for (int n = 0; n <= num_up; n++) {
        P[n] = (double*)calloc(max_5star + 1, sizeof(double));
        for (int m = 0; m <= max_5star; m++) {
            for (int c = 0; c < MAX_COUNTER; c++) {
                P[n][m] += dp[n][m][c];
            }
        }
    }

    // 释放 dp
    for (int n = 0; n <= num_up; n++) {
        for (int m = 0; m <= max_5star; m++) free(dp[n][m]);
        free(dp[n]);
    }
    free(dp);

    return P;
}

/* ============================================================
 *  第二层：复合层
 *  把 P_5star[n][m] 和 B^{*m}（m个5星的抽数卷积）复合
 *  得到 P(抽 j 抽出 n 个UP)
 * ============================================================ */

 // ben: 单个5星抽数分布，长度 pity+1
 // P_5star: [num_up+1][max_5star+1]
 // 返回：抽数分布，长度 max_pull+1
static double* compose_up_dist(double** P_5star, int num_up,
    const double* ben, int pity,
    int max_5star, int* out_max_pull) {
    int max_pull = max_5star * pity;   // 最坏情况每5星都吃满保底
    *out_max_pull = max_pull;

    // 预算 B_power[m] = B 的 m 次卷积
    // B_power[1] = B，B_power[m] = B_power[m-1] * B
    double** B_power = (double**)calloc(max_5star + 1, sizeof(double*));
    B_power[0] = (double*)calloc(1, sizeof(double));
    B_power[0][0] = 1.0;  // 0个5星消耗0抽

    for (int m = 1; m <= max_5star; m++) {
        int len_prev = (m - 1) * pity + 1;
        int len_cur = m * pity + 1;
        B_power[m] = (double*)calloc(len_cur, sizeof(double));
        convolve(B_power[m - 1], len_prev, ben, pity + 1, B_power[m]);
    }

    // 复合
    double* result = (double*)calloc(max_pull + 1, sizeof(double));
    for (int m = 0; m <= max_5star; m++) {
        double p_m = P_5star[num_up][m];
        if (p_m == 0.0) continue;
        int len = m * pity + 1;
        for (int j = 0; j < len; j++) {
            result[j] += p_m * B_power[m][j];
        }
    }

    // 释放
    for (int m = 0; m <= max_5star; m++) free(B_power[m]);
    free(B_power);

    return result;
}

/* ============================================================
 *  辅助：打印分布、统计量
 * ============================================================ */

static void print_dist(const char* title, const double* dist, int max_len) {
    printf("\n=== %s ===\n", title);
    double cum = 0.0;
    for (int i = 1; i <= max_len; i++) {
        if (dist[i] > 1e-12) {
            cum += dist[i];
            printf("%d | %.10g | cum=%.10g\n", i, dist[i], cum);
        }
    }
}

static void print_stats(const double* dist, int max_len) {
    double exp = 0.0, var = 0.0;
    for (int i = 0; i <= max_len; i++) exp += i * dist[i];
    for (int i = 0; i <= max_len; i++) var += (i - exp) * (i - exp) * dist[i];
    double sd = sqrt(var);

    printf("\n=== Statistics ===\n");
    printf("Expectation:        %.6f\n", exp);
    printf("Variance:        %.6f\n", var);
    printf("Standard Deviation:      %.6f\n", sd);
    printf("Average possibility:    %.6f\n", 1.0 / exp);
    printf("1sigma :     (%.3f, %.3f)\n", exp - sd, exp + sd);
    printf("2sigma :     (%.3f, %.3f)\n", exp - 2 * sd, exp + 2 * sd);
    printf("3sigma :     (%.3f, %.3f)\n", exp - 3 * sd, exp + 3 * sd);
}

/* ============================================================
 *  主流程
 * ============================================================ */

int main(void) {
    printf("========================================\n");
    printf("Calculator for Genshin Impact after 5.0:\n");
    printf("========================================\n");
    printf("targeted UP character num: %d\n", NUM_CHAR);
    printf("Capturing radiance init count: %d\n", CR_INIT);
    printf("CR_P : [%.2f, %.2f, %.2f, %.2f]\n",
        CR_P[0], CR_P[1], CR_P[2], CR_P[3]);

    /* ---------- 角色池 ---------- */
    double char_pity[PITY_CHAR + 1] = { 0 };
    double char_ben[PITY_CHAR + 1] = { 0 };
    build_char_pity(char_pity);
    build_ben(char_pity, char_ben, PITY_CHAR);

    // 校验 ben 归一化
    double sum_ben = 0.0;
    for (int i = 1; i <= PITY_CHAR; i++) sum_ben += char_ben[i];
    printf("\nsingle five star pulls cum = %.10f (should be 1)\n", sum_ben);

    // 第一层：状态机
    int max_5star = 0;
    double** P_5star = compute_5star_dist(NUM_CHAR, CR_INIT, &max_5star);
    double s = 0; for (int m = 0; m <= max_5star; m++) s += P_5star[NUM_CHAR][m];
    printf(" P_5star[%d] cum = %.10f (should be 1)\n", NUM_CHAR, s);

    // 打印 P_5star 分布
    printf("\n=== P_5star[%d][m]：targeted %d UP characters within m 5 stars ===\n",
        NUM_CHAR, NUM_CHAR);
    for (int m = 0; m <= max_5star; m++) {
        if (P_5star[NUM_CHAR][m] > 1e-12)
            printf("m = %2d | %.10g\n", m, P_5star[NUM_CHAR][m]);
    }

    // 第二层：复合
    int max_pull = 0;
    double* char_dist = compose_up_dist(P_5star, NUM_CHAR,
        char_ben, PITY_CHAR,
        max_5star, &max_pull);

    double cumulative[180 * NUM_CHAR + 1] = { 0.0 };
    printf("\n====possibility distribution====\n");
    for (int i = 1; i <= 180 * NUM_CHAR; i++) {
        cumulative[i] = cumulative[i - 1] + char_dist[i];
        printf("%d | %g\n", i, char_dist[i]);
    }
    printf("\n");

    printf("\n==== cumulative distribution====\n");
    for (int i = 1; i <= 180 * NUM_CHAR; i++) {
        printf("%d | %g\n", i, cumulative[i]);
    }
    printf("\n");

    print_stats(char_dist, max_pull);

    printf("\n===== percentages: =====\n");
    double p_arr[] = { 0.01,0.05,0.1,0.2,0.25,0.3,0.4,0.5,0.6,0.7,0.75,0.8,0.85,0.9,0.95,0.99 };
    for (int k = 0; k < length(p_arr); k++) {
        for (int i = 1; i <= 180 * NUM_CHAR; i++) {
            if (cumulative[i] >= p_arr[k]) {
                printf("%.1lf%% : %d pulls\n", 100 * p_arr[k], i);
                break;
            }
        }
    }

    /* ---------- 武器池（可选） ---------- */
#if CALC_GEAR
    printf("\n\n========================================\n");
    printf("  weapon banner(after 5.0)\n");
    printf("========================================\n");
    // TODO: 武器池定轨同样是状态相关，需要类似的二维DP
    // 这里先留空，需要时再实现
#endif

    /* ---------- 清理 ---------- */
    for (int n = 0; n <= NUM_CHAR; n++) free(P_5star[n]);
    free(P_5star);
    free(char_dist);

    return 0;
}
