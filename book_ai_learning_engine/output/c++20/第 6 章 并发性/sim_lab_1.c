// compile: g++ -std=c++20 -pthread -o demo concurrent_demo.cpp

#include <iostream>
#include <vector>
#include <latch>
#include <barrier>
#include <thread>
#include <random>
#include <chrono>

using namespace std::chrono_literals;

// 工作者函数：先等待启动栅栏，然后执行多轮计算，每轮末在 barrier 同步
void worker(int id, std::latch& startLatch, std::barrier<>& syncBarrier) {
    // 等待所有线程就绪
    startLatch.arrive_and_wait();

    // 模拟局部随机种子
    std::mt19937 rng(id + 100);
    std::uniform_int_distribution<int> dist(50, 200);

    for (int round = 0; round < 3; ++round) {
        // 模拟耗时计算
        std::this_thread::sleep_for(std::chrono::milliseconds(dist(rng)));
        std::osyncstream(std::cout) << "Worker " << id << " done round " << round << '\n';

        // 到达屏障并等待其他工作者，完成回调可观察到所有线程到达
        syncBarrier.arrive_and_wait();
    }
}

int main() {
    constexpr int N = 4;

    // 一次性启动栅栏，N 个工作者 + 主线程（主线程不用参与工作，本处仅用于演示管理）
    std::latch startLatch(N + 1);

    // 多阶段屏障，设置完成回调：每轮结束时主线程记录阶段完成
    int phase = 0;
    std::barrier syncBarrier(N, [&phase, N]() noexcept {
        ++phase;
        std::cout << "=== Phase " << phase << " completed by all " << N << " workers ===\n";
    });

    std::vector<std::jthread> workers;
    workers.reserve(N);
    for (int i = 0; i < N; ++i) {
        workers.emplace_back(worker, i, std::ref(startLatch), std::ref(syncBarrier));
    }

    // 主线程先到达启动栅栏，释放所有工作者（这里为了演示主线程等待，也可以单独用 latch）
    startLatch.arrive_and_wait();

    // jthread 自动 join，主线程无需手动 join
    return 0;
}