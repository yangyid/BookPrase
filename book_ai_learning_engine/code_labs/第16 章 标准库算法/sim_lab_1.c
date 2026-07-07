// algo_demo.cpp - 编译：g++ -std=c++11 algo_demo.cpp -o algo_demo
#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <iterator>
#include <string>
#include <cstdlib>
#include <ctime>

int main() {
    // 1. 生成随机序列
    std::srand(std::time(nullptr));
    std::vector<int> v(15);
    std::generate(v.begin(), v.end(), []{ return std::rand() % 100; });

    std::cout << "原始序列: ";
    std::copy(v.cbegin(), v.cend(), std::ostream_iterator<int>(std::cout, " "));

    // 2. 查找第一个 >50 的元素
    auto it = std::find_if(v.cbegin(), v.cend(), [](int i){ return i > 50; });
    std::cout << "\n第一个 >50 的元素: " << (it != v.cend() ? std::to_string(*it) : "未找到");

    // 3. 统计大于50的个数
    int cnt = std::count_if(v.cbegin(), v.cend(), [](int i){ return i > 50; });
    std::cout << "\n>50 的元素个数: " << cnt;

    // 4. 全部元素乘2变换，放入新容器
    std::vector<int> doubled;
    std::transform(v.cbegin(), v.cend(), std::back_inserter(doubled),
                   [](int i){ return i * 2; });
    std::cout << "\n全部乘2: ";
    std::copy(doubled.cbegin(), doubled.cend(), std::ostream_iterator<int>(std::cout, " "));

    // 5. 排序（升序）
    std::sort(v.begin(), v.end());
    std::cout << "\n升序排序: ";
    for(auto x : v) std::cout << x << ' ';

    // 6. 移除所有偶数（逻辑移除，配合erase）
    auto newEnd = std::remove_if(v.begin(), v.end(), [](int i){ return i%2 == 0; });
    v.erase(newEnd, v.end());
    std::cout << "\n移除偶数后: ";
    std::copy(v.cbegin(), v.cend(), std::ostream_iterator<int>(std::cout, " "));

    // 7. 累加求和
    int sum = std::accumulate(v.cbegin(), v.cend(), 0);
    std::cout << "\n累加和: " << sum;

    // 8. 用 for_each 格式化输出
    std::cout << "\n用 for_each 输出 (元素*10): ";
    std::for_each(v.cbegin(), v.cend(), [](int i){ std::cout << i*10 << ' '; });
    std::cout << std::endl;

    return 0;
}