#include <concepts>
#include <iostream>

template<typename T>
concept Addable = requires(T a, T b) {
    { a + b } -> std::same_as<T>;
};

template<Addable T>
T sum(const std::vector<T>& v) {
    T total{};
    for (auto& e : v) total = total + e;
    return total;
}

int main() {
    std::vector<int> vi{1,2,3};
    std::cout << sum(vi) << '\n'; // OK
    // std::vector<std::string> vs {"a","b"}; sum(vs); // 编译错误：std::string不满足Addable
}