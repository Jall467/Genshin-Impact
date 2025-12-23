#include <iostream>
#include <vector>
#include <iomanip>
#include <map>
#include <bitset>
#include <functional>

using namespace std;

// 物品数据
const vector<int> weights = {1,3,10,10,25,25,25,50,50,75,75,85,85,99,99};
const vector<int> costs = {90,150,200,250,300,380,460,560,660,760,960,1160,1360,1560,1760};
const int n = 15;

// 记忆化存储
map<pair<int, int>, long double> memo;

// ========== 可配置的停止条件函数 ==========
// 在这里修改停止条件逻辑
function<bool(int)> stop_condition = [](int mask) {
    // 示例1：只抽到第一个物品（原问题）
    // return (mask & 1);  // 第0位为1表示抽到第一个物品

    // 示例2：抽到第一个和第二个物品
    return (mask & 1) && (mask & 2) ;  // 第0位和第1位都为1

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

// ========== 递归计算概率 ==========
// 计算：从状态mask开始，还需要step步时停止的概率
long double dfs(int mask, int step) {
    // 如果当前已经满足停止条件
    if (stop_condition(mask)) {
        // 还需要0步就停止（已经停止了）
        return (step == 0) ? 1.0L : 0.0L;
    }

    // 如果已经没有步数了但还没满足条件
    if (step == 0) return 0.0L;

    // 如果所有物品都抽完了但还没满足条件
    if (bitset<32>(mask).count() == n) return 0.0L;

    // 记忆化检查
    auto key = make_pair(mask, step);
    if (memo.find(key) != memo.end()) return memo[key];

    // 计算当前状态的总权重
    int total_weight = 0;
    for (int i = 0; i < n; i++) {
        if (!(mask & (1 << i))) {
            total_weight += weights[i];
        }
    }

    long double prob = 0.0L;

    // 尝试抽取每个未抽到的物品
    for (int i = 0; i < n; i++) {
        if (!(mask & (1 << i))) {
            int new_mask = mask | (1 << i);
            long double p = (long double)weights[i] / total_weight;

            // 继续递归，步数减1
            prob += p * dfs(new_mask, step - 1);
        }
    }

    memo[key] = prob;
    return prob;
}

// ========== 主程序 ==========
int main() {
    cout << "====== 精确概率计算（可配置停止条件）======\n" << endl;

    // 显示当前停止条件
    cout << "当前停止条件：";
    if (stop_condition(1)) {
        cout << "抽到物品1（权重1）" << endl;
    } else if (stop_condition(3)) {  // 3的二进制是11，表示物品1和2都被抽到
        cout << "抽到物品1（权重1）和物品2（权重10）" << endl;
    } else {
        cout << "自定义停止条件" << endl;
    }
    cout << endl;

    // 计算第1抽到第15抽结束的概率
    vector<long double> probs(16, 0.0L);
    long double total_prob = 0.0L;

    cout << "正在计算概率分布..." << endl;

    // 对于每个可能的抽取次数
    for (int k = 1; k <= n; k++) {
        memo.clear();  // 清除记忆化缓存
        probs[k] = dfs(0, k);  // 从初始状态开始，k步内停止的概率

        // 注意：这里计算的是"恰好第k次抽取时停止"的概率
        // 也就是：前k-1次没停止，第k次停止

        total_prob += probs[k];
    }

    // 输出结果
    cout << fixed << setprecision(10);
    cout << "\n====== 结果 ======\n" << endl;

    bool has_output = false;
    for (int k = 1; k <= n; k++) {
        if (probs[k] >= 0) {  // 只显示有显著概率的情况
            has_output = true;
            cout << "第" << setw(2) << k << "抽结束的概率: "
                 << probs[k] << " (" << probs[k]*100 << "%)" << endl;
        }
    }

    if (!has_output) {
        cout << "警告：没有计算出任何有效概率！" << endl;
        cout << "可能是停止条件设置有问题。" << endl;
        return 1;
    }

    cout << "\n概率总和: " << total_prob << endl;
    cout << "验证: " << (abs(total_prob - 1.0L) < 1e-10 ? " 正确" : " 错误") << endl;

    if (abs(total_prob - 1.0L) > 1e-10) {
        cout << "警告：概率总和不为1，可能是算法有误或停止条件不可能达成！" << endl;
    }

    // 计算期望花费
    long double expected_cost = 0.0L;
    int cumulative_costs[16] = {0};

    // 计算累计花费
    cumulative_costs[1] = costs[0];
    for (int k = 2; k <= n; k++) {
        cumulative_costs[k] = cumulative_costs[k-1] + costs[k-1];
    }

    for (int k = 1; k <= n; k++) {
        expected_cost += probs[k] * cumulative_costs[k];
    }

    cout << fixed << setprecision(2);
    cout << "\n期望花费: " << expected_cost << endl;

    // 累积分布函数
    cout << "\n累积分布函数（CDF）：" << endl;
    long double cumulative = 0.0L;
    for (int k = 1; k <= n; k++) {
        if (probs[k] > 1e-15) {
            cumulative += probs[k];
            cout << "P(≤" << setw(2) << k << "抽): "
                 << fixed << setprecision(6) << cumulative * 100 << "%" << endl;
        }
    }

    return 0;
}
