# Genshin-Impact
#include<stdio.h>
#include<string.h>
double benchou(int n, double p[]) {
    double mult = 1.0;
    if (n == 1) {
        mult = 0.006;
    }
    else {
        for (int k = 1; k < n; k++) {
            mult = mult * (1 - p[k]);
            if (k == n - 1) {
                mult = mult * p[n];
            }
        }
    }
    return mult;
}
double upwai(int n, double p[]) {
    double proba = 0.0;
    if (n >= 2 && n <= 90) {
        for (int k = 1; k <= n - 1; k++) {
            proba = proba + benchou(k, p) * 0.5 * benchou(n - k, p);
        }
    }
    else if (n > 90 && n <= 180) {
        for (int k = 1; k <= 90; k++) {
            if (n - k <= 90) {
                proba = proba + benchou(k, p) * 0.5 * benchou(n - k, p);
            }
        }
    }
    return proba;
}
double nchoubuchu(int n, double p[]) {
    return benchou(n + 1, p) / p[n + 1];
}
double upleijinwai(int n, double p[]) {
    double prob = 0.0;
    if (n >= 2 && n <= 90) {
        for (int k = 1; k <= n - 1; k++) {
            prob = prob + benchou(k, p) * 0.5 * nchoubuchu(n - k, p);
        }
    }
    else if (n > 90 && n <= 180) {
        for (int k = 1; k <= 90; k++) {
            if (n - k < 90) {
                prob = prob + benchou(k, p) * 0.5 * nchoubuchu(n - k, p);
            }
        }
    }
    return prob;
}
int main() {
    double pos[91];
    for (int j = 1; j <= 73; j++) {
        pos[j] = 0.006;
    }
    for (int i = 74; i <= 90; i++) {
        pos[i] = 0.006 + 0.06 * (i - 73);
    }
    pos[90] = 1.0;
    double pro[181]; double leijin[181];
    for (int a = 1; a <= 180; a++) {//计算恰好在第n抽出货的概率
        if (a == 1) {
            pro[a] = benchou(a, pos) * 0.5;
        }
        else if (a >= 2 && a <= 90) {
            pro[a] = benchou(a, pos) * 0.5 + upwai(a, pos);
        }
        else if (a > 90) {
            pro[a] = upwai(a, pos);
        }
    }
    for (int a = 1; a <= 180; a++) {//计算n抽不出货的概率
        if (a == 1) {
            leijin[a] = 1.0 - benchou(a, pos) * 0.5;
        }
        else if (a >= 2 && a < 90) {
            leijin[a] = nchoubuchu(a, pos) + upleijinwai(a, pos) + benchou(a, pos) * 0.5;
        }
        else if (a == 90) {
            leijin[a] = benchou(a, pos) * 0.5 + upleijinwai(a, pos);
        }
        else if (a > 90) {
            leijin[a] = upleijinwai(a, pos);
        }
    }
    const int NUM_CHARS = 1;
    const int MAX_SUM = 180 * NUM_CHARS;
    double dp[NUM_CHARS + 1][MAX_SUM + 1];
    memset(dp, 0, sizeof(dp));
    dp[0][0] = 1.0;
    for (int i = 1; i <= NUM_CHARS; i++) {  // 逐个添加角色
        for (int j = 0; j <= MAX_SUM; j++) {  // 当前总抽数
            if (dp[i - 1][j] > 0) {  // 如果前i-1个角色j抽的概率不为0
                for (int k = 1; k <= 180; k++) {  // 当前角色抽数
                    if (j + k <= MAX_SUM) {
                        dp[i][j + k] += dp[i - 1][j] * pro[k];
                    }
                }
            }
        }
    }
    double xinghun[MAX_SUM + 1] = { 0.0 };
    for (int j = 0; j <= MAX_SUM; j++) {
        xinghun[j] = dp[NUM_CHARS][j];
    }
    double leijinxinghun[MAX_SUM + 1] = { 0.0 };
    for (int i = 1; i <= MAX_SUM; i++) {
        leijinxinghun[i] = leijinxinghun[i - 1] + xinghun[i];
    }
    double total_prob = 0.0;
    for (int i = 1; i <= MAX_SUM; i++) {
        total_prob += xinghun[i];
    }
    double sum = 0.0;
    for (int b = 1; b <= 180; b++) {
        sum += pro[b];
    }
    //for (int a = 1; a <= 180; a++) {
    //    printf("%d %.16lf\n", a, pro[a]);
    //}
    //printf("\n");
    double exp = 0.0;
    //for (int a = 1; a <= 180; a++) {
    //    exp = exp + a * pro[a];
    //}
    //for (int a = 1; a <= 180; a++) {
    //    printf("%d %.16lf\n", a, 1 - leijin[a]);
    //}
    //printf("sum:%lf\n", sum);
    //printf("expectation:%lf\n", exp);
    //printf("average possibility:%f\n", 1 / exp);
    //printf("\n");
    for (int i = 1; i <= MAX_SUM; i++) {
        printf("%d %.50lf\n", i, xinghun[i]);
    }
    printf("\n");
    for (int i = 1; i <= MAX_SUM; i++) {
        printf("%d %.50lf\n", i, leijinxinghun[i]);
    }
    printf("Total probability: %.16lf\n", total_prob);
    return 0;
}
