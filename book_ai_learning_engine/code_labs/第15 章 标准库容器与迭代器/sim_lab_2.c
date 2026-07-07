#include <iostream>
#include <vector>

int main() {
    std::vector<int> v {1,2,3};
    auto it = v.begin() + 1; // 指向元素2
    std::cout << "Initial capacity: " << v.capacity() << ", *it = " << *it << '\n';

    v.push_back(4);
    v.push_back(5); // 可能触发扩容
    std::cout << "After pushes, capacity: " << v.capacity() << '\n';

    // 危险！it可能已失效，访问未定义行为
    // std::cout << *it << '\n'; // 注释掉以免崩溃
    // 正确做法：重新获取迭代器
    auto new_it = v.begin() + 1;
    std::cout << "Valid *new_it = " << *new_it << '\n';

    return 0;
}