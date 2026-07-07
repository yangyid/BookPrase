#include <iostream>
#include <span>
#include <vector>

// 特性测试宏：C++20 概念
#ifdef __cpp_concepts
#  define HAS_CONCEPTS 1
#else
#  define HAS_CONCEPTS 0
#endif

// 有条件地使用概念约束
#if HAS_CONCEPTS
#  include <concepts>
template <typename T>
concept Numeric = std::integral<T> || std::floating_point<T>;

template <Numeric T>
T add(T a, T b) { return a + b; }
#else
// 传统 SFINAE 替代
template <typename T, typename = std::enable_if_t<std::is_arithmetic_v<T>>>
T add(T a, T b) { return a + b; }
#endif

int main() {
    std::cout << "C++ standard version: " << __cplusplus << std::endl;
    std::cout << "Concepts support: " << std::boolalpha << (bool)HAS_CONCEPTS << std::endl;

    // C++20 std::span 测试
    std::vector<int> v = {1, 2, 3, 4, 5};
    std::span<int> s(v);
    std::cout << "First element via span: " << s.front() << std::endl;

    std::cout << "add(3.1, 4.2): " << add(3.1, 4.2) << std::endl;
    return 0;
}