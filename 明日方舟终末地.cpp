#include"stdio.h"
#include"math.h"
#include<vector>
const int so_pity_first = 90;
const int so_pity_normal = 80;
const int count_layer = 30;
const int max_draws = 2000;  // 扩大上限
void calc_first_six_dist(double* p6, int limit, bool has_free_10) {
    double single_prob[200] = {0.0};
    if (has_free_10) {
        for (int i = 1; i <= 30; i++) single_prob[i] = 0.008;
        for (int i = 31; i <= 40; i++) single_prob[i] = 0.008;
        for (int i = 41; i <= 75; i++) single_prob[i] = 0.008;
        for (int i = 76; i <= 89; i++) {
            int counter = i - 10;
            single_prob[i] = 0.008 + 0.05 * (counter - 65);
        }
        single_prob[90] = 1.0;
    } else {
        for (int i = 1; i <= 65; i++) single_prob[i] = 0.008;
        for (int i = 66; i <= 79; i++) {
            single_prob[i] = 0.008 + 0.05 * (i - 65);
        }
        single_prob[80] = 1.0;
    }
    double survive = 1.0;
    for (int n = 1; n <= limit; n++) {
        p6[n] = survive * single_prob[n];
        survive *= (1.0 - single_prob[n]);
    }
}
int main() {
    double p6_first[200] = {0.0};
    calc_first_six_dist(p6_first, 90, true);
    double p6_normal[200] = {0.0};
    calc_first_six_dist(p6_normal, 80, false);
    std::vector<std::vector<double>> fail(count_layer + 1, std::vector<double>(max_draws, 0.0));
    std::vector<std::vector<double>> succ(count_layer + 1, std::vector<double>(max_draws, 0.0));
    fail[0][0] = 1.0;
    for (int layer = 0; layer < count_layer; layer++) {
        for (int total = 0; total < max_draws; total++) {
            if (fail[layer][total] <= 1e-15) continue;
            double* p6 = (layer == 0) ? p6_first : p6_normal;
            int limit = (layer == 0) ? 90 : 80;
            for (int k = 1; k <= limit; k++) {
                int new_total = total + k;
                if (new_total >= max_draws) continue;
                double prob_6 = p6[k];
                fail[layer + 1][new_total] += fail[layer][total] * prob_6 * 0.5;
                succ[layer + 1][new_total] += fail[layer][total] * prob_6 * 0.5;
            }
        }
    }
    double prob_by_draws[max_draws] = {0.0};
    for (int layer = 1; layer <= count_layer; layer++) {
        for (int d = 1; d < max_draws; d++) {
            prob_by_draws[d] += succ[layer][d];
        }
    }
    double final[121] = {0.0};
    double cum[121] = {0.0};

    for (int d = 1; d <= 30; d++) {
        final[d] = prob_by_draws[d];
    }
    for (int d = 31; d <= 40; d++) {
        final[30] += prob_by_draws[d];
    }
    for (int d = 41; d < max_draws; d++) {
        int consume = d - 10;
        if (consume <= 120) {
            final[consume] += prob_by_draws[d];
        } else {
            final[120] += prob_by_draws[d];
        }
    }
    double total = 0;
    for (int i = 1; i <= 120; i++) {
        total += final[i];
        cum[i] = cum[i-1] + final[i];
    }
    printf("Total probability: %lf\n", total);
    printf("\n=== Probability Distribution ===\n");
    for (int i = 1; i <= 120; i++) {
        if (final[i] > 1e-12) printf("%d | %lf\n", i, final[i]);
    }
    printf("\n=== Cumulative Distribution ===\n");
    for (int i = 1; i <= 120; i++) {
        if (cum[i] > 1e-12) printf("%d | %lf\n", i, cum[i]);
    }
    printf("\n===important points===\n");
    double p_arr[] = {0.01, 0.05, 0.1, 0.2, 0.25, 0.3, 0.4, 0.5, 0.6, 0.7, 0.75, 0.8, 0.85, 0.9, 0.95, 0.99};
    for (int k = 0; k < (sizeof(p_arr)/sizeof(p_arr[0])); k++) {
        for (int i = 1; i <= 120; i++) {
            if (cum[i] >= p_arr[k]) {
                printf("%.1lf%% at: %d\n", 100 * p_arr[k], i);
                break;
            }
        }
    }

    double exp = 0, var = 0;
    for (int i = 1; i <= 120; i++) {
        exp += i * final[i];
    }
    for (int i = 1; i <= 120; i++) {
        var += pow(i - exp, 2) * final[i];
    }
    printf("\n=== Statistics ===\n");
    printf("Hard pity (120 pulls) probability: %lf\n", final[120]);
    printf("Expectation: %lf\n", exp);
    printf("Variance: %lf\n", var);
    printf("Standard deviation: %lf\n", sqrt(var));
    return 0;
}
