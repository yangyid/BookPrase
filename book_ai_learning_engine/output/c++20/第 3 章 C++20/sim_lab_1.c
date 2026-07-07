#include <concepts>
#include <iostream>
#include <vector>
#include <list>

// 定义一个概念：可打印对象
template<typename T>
concept Printable = requires(const T& v) {
    { std::cout << v } -> std::same_as<std::ostream&>;
};

// 使用概念约束函数模板
template<Printable T>
void safePrint(const T& value) {
    std::cout << value << '\n';
}

// 定义一个概念：可比较的类型
template<typename T>
concept EqualityComparable = requires(const T& a, const T& b) {
    { a == b } -> std::convertible_to<bool>;
    { a != b } -> std::convertible_to<bool>;
};

// 约束模板类
template<EqualityComparable T>
class ValueWrapper {
    T data;
public:
    explicit ValueWrapper(T d) : data(std::move(d)) {}
    bool equals(const ValueWrapper& other) const {
        return data == other.data;
    }
};

int main() {
    safePrint(42);          // int 满足 Printable?
    // safePrint(std::vector{1,2,3}); // 编译错误：vector 不支持 <<

    ValueWrapper<int> w1(10), w2(20);
    std::cout << std::boolalpha << w1.equals(w2) << '\n';

    // 测试标准库概念：std::integral
    static_assert(std::integral<int>);
    // static_assert(std::integral<double>); // 触发编译错误

    return 0;
}