#include "stdio.h"
#include "stdlib.h"
#include "math.h"
#include "time.h"

#define SIM_TIMES 10000000  // 1000万次模拟
#define MAX_PITY 80
#define MAX_PULLS 60
#define MAX_COPIES 1

// 五星概率初始化
void init_five_star_probs(double* probs) {
    for (int i = 1; i <= 65; i++) probs[i] = 0.008;
    for (int i = 66; i <= 79; i++) probs[i] = 0.008 + 0.05 * (i - 65);
    probs[80] = 1.0;
}

// 单次模拟
int simulate_once(double* five_star_prob) {
    int copies = 0;
    int total_pulls = 0;
    int pity = 0;
    int since_last_up = 0;
    int phase = 0;  // 0=120抽保底生效期, 1=保底已用

    while (copies < MAX_COPIES && total_pulls < MAX_PULLS) {
        total_pulls++;
        pity++;
        since_last_up++;

        // 检查240抽固定命座（独立机制）
        if (total_pulls % 240 == 0 && copies < MAX_COPIES) {
            copies++;
            // 不重置since_last_up，独立机制
        }

        // 判断是否出五星
        double r = (double)rand() / RAND_MAX;
        if (r < five_star_prob[pity]) {
            // 出了五星

            // 判断是否UP
            int is_up = 0;

            if (phase == 0 && since_last_up >= 120) {
                // 触发120抽保底（第120抽必UP）
                is_up = 1;
                phase = 1;  // 保底已用
            }
            else {
                // 正常50%概率
                is_up = (rand() % 2 == 0);
            }

            if (is_up) {
                copies++;
                since_last_up = 0;  // 重置UP计数
            }
            // 非UP五星：不重置since_last_up

            pity = 0;  // 重置五星保底
        }

        if (pity >= MAX_PITY) pity = MAX_PITY;
    }

    return copies;
}

int main() {
    srand(time(NULL));

    // 初始化五星概率
    double five_star_prob[MAX_PITY + 1] = { 0.0 };
    init_five_star_probs(five_star_prob);

    // 结果统计
    long long results[MAX_COPIES + 1] = { 0 };
    int* pulls_to_six = (int*)malloc(SIM_TIMES * sizeof(int));
    int valid_sims = 0;

    printf("正在进行蒙特卡洛模拟 (%d次)...\n", SIM_TIMES);

    for (int sim = 0; sim < SIM_TIMES; sim++) {
        if (sim % 1000000 == 0 && sim > 0) {
            printf("已完成 %d 万次模拟\n", sim / 10000);
        }

        int copies = 0;
        int total_pulls = 0;
        int pity = 0;
        int since_last_up = 0;
        int phase = 0;

        while (copies < MAX_COPIES && total_pulls < MAX_PULLS) {
            total_pulls++;
            pity++;
            since_last_up++;

            // 240抽固定命座
            if (total_pulls % 240 == 0 && copies < MAX_COPIES) {
                copies++;
            }

            // 出五星判定
            double r = (double)rand() / RAND_MAX;
            if (r < five_star_prob[pity]) {
                int is_up = 0;

                if (phase == 0 && since_last_up >= 120) {
                    is_up = 1;
                    phase = 1;
                }
                else {
                    is_up = (rand() % 2 == 0);
                }

                if (is_up) {
                    copies++;
                    since_last_up = 0;
                }

                pity = 0;
            }

            if (pity >= MAX_PITY) pity = MAX_PITY;
        }

        if (copies >= MAX_COPIES) {
            results[MAX_COPIES]++;
            pulls_to_six[valid_sims++] = total_pulls;
        }
        else {
            results[copies]++;
        }
    }

    printf("\n========== 结果分析 ==========\n");
    printf("总模拟次数: %d\n", SIM_TIMES);
    printf("满命次数: %d (%.2f%%)\n", results[MAX_COPIES],
        100.0 * results[MAX_COPIES] / SIM_TIMES);
    free(pulls_to_six);
    return 0;
}
