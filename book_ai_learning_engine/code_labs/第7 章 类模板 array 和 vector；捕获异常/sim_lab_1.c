#include <iostream>
#include <array>
#include <vector>
#include <stdexcept>
#include <string>

// 自定义异常类
class ContainerException : public std::runtime_error {
public:
    explicit ContainerException(const std::string& msg)
        : std::runtime_error("ContainerException: " + msg) {}
};

// 一个有异常安全保护的函数：向 vector 添加元素
void safeAdd(std::vector<int>& vec, int val) {
    if (val < 0) {
        throw ContainerException("Negative value not allowed");
    }
    vec.push_back(val);
}

int main() {
    // ---- std::array 演示 ----
    std::array<int, 4> arr = {1, 2, 3, 4};
    std::cout << "array size: " << arr.size() << '\n';
    try {
        std::cout << "arr.at(4) = " << arr.at(4) << '\n';  // 越界，抛出 std::out_of_range
    } catch (const std::out_of_range& e) {
        std::cerr << "Caught out_of_range: " << e.what() << '\n';
    }
    // 不检查越界，未定义行为（注释掉以避免崩溃）
    // std::cout << "arr[4] = " << arr[4] << '\n';

    // ---- std::vector 演示 ----
    std::vector<int> vec{10, 20, 30};
    std::cout << "vector size: " << vec.size() << ", capacity: " << vec.capacity() << '\n';
    vec.push_back(40);
    std::cout << "After push_back, size: " << vec.size() << ", capacity: " << vec.capacity() << '\n';

    // 调用可能抛出自定义异常的函数
    try {
        safeAdd(vec, -5);
    } catch (const ContainerException& e) {
        std::cerr << e.what() << '\n';
    } catch (const std::exception& e) {
        std::cerr << "Standard exception: " << e.what() << '\n';
    }

    // 使用 at() 安全访问
    try {
        std::cout << "vec.at(10) = " << vec.at(10) << '\n';
    } catch (const std::out_of_range& e) {
        std::cerr << "Out of range access prevented: " << e.what() << '\n';
    }

    return 0;
}