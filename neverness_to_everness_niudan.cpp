#include <iostream>
#include <vector>
#include <iomanip>
#include <map>
#include <cmath>
#include <functional>
using namespace std;
// 物品数据
const vector<int> weights = { 1,3,5,10,10,99,99,99,99,99,99,99,99,99,99 };
const vector<int> costs1 = { 50,80,100,150,200,300,500,600,800,1200,1400,1600,1800,2000,2200 };
const vector<int> costs2 = { 50,80,100,150,200,300,500,800,1000,1500,2000,2500,3000,3500,4200 };
const int n = 15;

// 记忆化存储
map<pair<int, int>, long double> memo;

// ========== 可配置的停止条件函数 ==========
function<bool(int)> stop_condition = [](int mask) {
    // 示例1：只抽到第一个物品（原问题）
    return (mask & 1);  // 第0位为1表示抽到第一个物品

    // 示例2：抽到第一个和第二个物品
    //return (mask & 1) && (mask & 2) ;  // 第0位和第1位都为1

    // 示例3：抽到任意指定物品
    // return (mask & 1) || (mask & 4);  // 抽到物品1或物品3

    // 示例4：抽到所有物品
    // return mask == ((1 << n) - 1);  // 所有位都为1
    /*stop_condition函数详解
    这个函数是停止条件判断器，它根据当前已抽取的物品集合（用位掩码mask表示）判断是否应该停止抽取。
    位掩码mask的含义
    mask是一个整数，它的二进制表示有15位（对应15个物品）
    第i位为1表示第i个物品已被抽到（从0开始计数）
    例如：
        mask = 1（二进制000000000000001）表示只抽到了物品0
        mask = 3（二进制000000000000011）表示抽到了物品0和物品1
        mask = 5（二进制000000000000101）表示抽到了物品0和物品2*/
    };

// ========== 检查物品在当前步数是否可用 ==========
bool is_item_available(int step, int item_index) {
    // 物品0（权重1）只在第6抽及以后才可用（step从1开始）
    if (item_index == 0) {
        return step >= 6;
    }
    // 其他物品始终可用
    return true;
}

// ========== 递归计算：恰好经过step步后首次满足停止条件的概率 ==========
// step: 当前已进行的抽取次数（1-based）
// target_step: 目标停止步数
// mask: 当前已抽到的物品集合
long double dfs_first_stop(int mask, int step, int target_step) {
    // 如果已经满足停止条件，说明之前就已经停止了
    if (stop_condition(mask)) {
        return 0.0L;
    }

    // 如果已经达到目标步数
    if (step == target_step) {
        return 0.0L;
    }
    // 检查是否还有可用物品（考虑步数限制）
    bool has_available = false;
    int total_weight = 0;
    for (int i = 0; i < n; i++) {
        if (!(mask & (1 << i)) && is_item_available(step + 1, i)) {
            has_available = true;
            total_weight += weights[i];
        }
    }
    if (!has_available) return 0.0L;
    // 记忆化检查
    auto key = make_pair(mask, step * 100 + target_step);
    if (memo.find(key) != memo.end()) return memo[key];

    long double prob = 0.0L;

    // 尝试抽取每个可用的物品
    for (int i = 0; i < n; i++) {
        if (!(mask & (1 << i)) && is_item_available(step + 1, i)) {
            int new_mask = mask | (1 << i);
            long double p = (long double)weights[i] / total_weight;

            // 如果抽到这个物品后满足停止条件
            if (stop_condition(new_mask)) {
                // 且这正好是第target_step次抽取
                if (step + 1 == target_step) {
                    prob += p;
                }
                // 否则，提前停止，不计入
            }
            else {
                // 没满足条件，继续递归
                prob += p * dfs_first_stop(new_mask, step + 1, target_step);
            }
        }
    }
    memo[key] = prob;
    return prob;
}

// ========== 主程序 ==========
int main() {
    cout << "====== 精确概率计算 ======\n" << endl;
    // 计算第1抽到第15抽结束的概率
    vector<long double> probs(16, 0.0L);
    long double total_prob = 0.0L;

    cout << "正在计算概率分布..." << endl;

    // 对于每个可能的抽取次数
    for (int k = 1; k <= n; k++) {
        memo.clear();  // 清除记忆化缓存
        probs[k] = dfs_first_stop(0, 0, k);  // 从初始状态开始，恰好k步时首次停止的概率
        total_prob += probs[k];

        if (k % 3 == 0) {
            cout << "  已完成 " << k << "/" << n << " 步的计算..." << endl;
        }
    }

    // 输出结果
    cout << fixed << setprecision(10);
    cout << "\n====== 结果 ======\n" << endl;

    bool has_output = false;
    for (int k = 1; k <= n; k++) {
        has_output = true;
        cout << "第" << setw(2) << k << "抽抽到的概率: "
                << setw(12) << probs[k] << " (" << setw(10) << probs[k] * 100 << "%)" << endl;
    }

    if (!has_output) {
        cout << "警告：没有计算出任何有效概率！" << endl;
        return 1;
    }

    cout << "\n概率总和: " << total_prob << endl;
    cout << "验证: " << (abs(total_prob - 1.0L) < 1e-10 ? "正确" : "错误") << endl;

    // 计算期望花费
    long double expected_cost1 = 0.0L, expected_cost2 = 0.0L;
    int cumulative_costs1[16] = { 0 };
    int cumulative_costs2[16] = { 0 };

    // 计算累计花费
    cumulative_costs1[0] = 0;
    cumulative_costs2[0] = 0;
    for (int k = 1; k <= n; k++) {
        cumulative_costs1[k] = cumulative_costs1[k - 1] + costs1[k - 1];
        cumulative_costs2[k] = cumulative_costs2[k - 1] + costs2[k - 1];
    }

    for (int k = 1; k <= n; k++) {
        expected_cost1 += probs[k] * cumulative_costs1[k];
        expected_cost2 += probs[k] * cumulative_costs2[k];
    }
    cout << fixed << setprecision(2);
    cout << "\n========== 期望花费 ==========" << endl;
    cout << "环石期望花费: " << expected_cost1 << endl;
    cout << "方斯期望花费: " << expected_cost2 * 1000 << endl;
    cout << "\n========== 累积分布函数（CDF） ==========" << endl;
    long double cumulative = 0.0L;
    for (int k = 1; k <= n; k++) {
        cumulative += probs[k];
        cout << "P(≤" << setw(2) << k << "抽): "
            << fixed << setprecision(6) << setw(10) << cumulative * 100 << "%";
        cout << "     环石累计花费：" << cumulative_costs1[k] << "     " << "方斯累计花费：" << cumulative_costs2[k] * 1000;
        cout << endl;
    }
    // 额外统计信息
    cout << "\n========== 统计信息 ==========" << endl;
    // 找到第一个非零概率
    int first_nonzero = 1;
    for (int k = 1; k <= n; k++) {
        if (probs[k] > 1e-15) {
            first_nonzero = k;
            break;
        }
    }
    cout << "最小抽取次数: " << first_nonzero << " 抽（因为前5抽不可能抽到物品0）" << endl;

    // 找到最可能抽取次数
    int max_prob_step = first_nonzero;
    long double max_prob = probs[first_nonzero];
    for (int k = first_nonzero + 1; k <= n; k++) {
        if (probs[k] > max_prob) {
            max_prob = probs[k];
            max_prob_step = k;
        }
    }
    cout << "最可能抽取次数: " << max_prob_step << " 抽 (概率: " << max_prob * 100 << "%)" << endl;
    // 验证前5抽概率是否为0
    double sum_first5 = 0;
    for (int k = 1; k <= 5; k++) {
        sum_first5 += probs[k];
    }
    cout << "\n前5抽总概率: " << sum_first5 * 100 << "% "
        << (sum_first5 < 1e-10 ? " 正确（为0）" : " 错误（应为0）") << endl;

    return 0;
}
