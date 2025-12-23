#include <iostream>
#include <vector>
#include <random>
#include <numeric>
#include <chrono>

// 使用thread_local确保每个线程有独立的随机数生成器
thread_local std::mt19937 gen(std::random_device{}());

int test() {
    std::vector<int> weights = {1,10,10,10,25,25,25,50,50,75,75,85,85,99,99};                 //角色高定扭蛋机权重向量第二个为3，载具扭蛋机或时装扭蛋机权重向量第二个为10，记得看清楚所有物品的权重各为多少
    std::vector<int> costs = {90,150,200,250,300,380,460,560,660,760,960,1160,1360,1560,1760};

    int cost = 0;
    int count = 0;

    //当第一个物品（权重为1）没有被抽到前继续抽取
    while(weights[0] !=0) {//抽取结束逻辑，如果是只要高定时装不要武器或只要载具不要时装的话条件是weights[0]!=0,若高定时装武器都要或载具时装都要则条件是weights[0] != 0 || weights[1] !=0
        // 计算总权重
        int total_weight = std::accumulate(weights.begin(), weights.end(), 0);

        // 创建轮盘赌概率分布
        std::discrete_distribution<> dist(weights.begin(), weights.end());

        // 抽取
        int prize = dist(gen);

        // 累加花费
        cost += costs[count];
        count++;

        // 抽到的物品权重设为0
        weights[prize] = 0;
    }

    return cost;
}

int main() {
    // 初始化计数器
    int p[15] = {0};
    // 每个抽取次数对应的总花费
    int total_costs[15] = {90,240,440,690,990,1370,1830,2390,3050,3810,4770,5930,7290,8850,10610};

    // 使用时间作为随机种子，确保每次运行不同
    auto seed = std::chrono::high_resolution_clock::now().time_since_epoch().count();
    gen.seed(seed);

    auto start = std::chrono::high_resolution_clock::now();

    const int iterations = 10000000;  // 模拟抽取次数设置
    for(int i = 0; i < iterations; i++) {
        int co = test();

        // 查找对应的抽数
        for(int j = 0; j < 15; j++) {
            if(co == total_costs[j]) {
                p[j]++;
                break;
            }
        }

        // 一定次数输出进度，可修改，一般设定为模拟抽取次数的十分之一
        if((i + 1) % 1000000 == 0) {
            std::cout << "已完成 " << (i + 1) << " 次模拟..." << std::endl;
        }
    }

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

    // 输出结果
    std::cout << "\n====== 最终结果 ======" << std::endl;
    std::cout << "模拟次数：" << iterations << std::endl;
    std::cout << "第1抽结束：" << p[0] << " (" << (p[0]*100.0/iterations) << "%)" << std::endl;
    std::cout << "第2抽结束：" << p[1] << " (" << (p[1]*100.0/iterations) << "%)" << std::endl;
    std::cout << "第3抽结束：" << p[2] << " (" << (p[2]*100.0/iterations) << "%)" << std::endl;
    std::cout << "第4抽结束：" << p[3] << " (" << (p[3]*100.0/iterations) << "%)" << std::endl;
    std::cout << "第5抽结束：" << p[4] << " (" << (p[4]*100.0/iterations) << "%)" << std::endl;
    std::cout << "第6抽结束：" << p[5] << " (" << (p[5]*100.0/iterations) << "%)" << std::endl;
    std::cout << "第7抽结束：" << p[6] << " (" << (p[6]*100.0/iterations) << "%)" << std::endl;
    std::cout << "第8抽结束：" << p[7] << " (" << (p[7]*100.0/iterations) << "%)" << std::endl;
    std::cout << "第9抽结束：" << p[8] << " (" << (p[8]*100.0/iterations) << "%)" << std::endl;
    std::cout << "第10抽结束：" << p[9] << " (" << (p[9]*100.0/iterations) << "%)" << std::endl;
    std::cout << "第11抽结束：" << p[10] << " (" << (p[10]*100.0/iterations) << "%)" << std::endl;
    std::cout << "第12抽结束：" << p[11] << " (" << (p[11]*100.0/iterations) << "%)" << std::endl;
    std::cout << "第13抽结束：" << p[12] << " (" << (p[12]*100.0/iterations) << "%)" << std::endl;
    std::cout << "第14抽结束：" << p[13] << " (" << (p[13]*100.0/iterations) << "%)" << std::endl;
    std::cout << "第15抽结束：" << p[14] << " (" << (p[14]*100.0/iterations) << "%)" << std::endl;

    int sum = 0;
    for(int i = 0; i < 15; i++) {
        sum += p[i];
    }
    std::cout << "累计验证：" << sum << std::endl;
    std::cout << "运行时间：" << duration.count() << " 毫秒" << std::endl;

    return 0;
}
