#include <ranges>
#include <vector>
#include <iostream>

int main() {
    std::vector<int> v = {1,2,3,4,5,6};
    auto even_squares = v | std::views::filter([](int x){ return x%2==0; })
                          | std::views::transform([](int x){ return x*x; });
    for (auto n : even_squares)
        std::cout << n << ' '; // 输出 4 16 36
}