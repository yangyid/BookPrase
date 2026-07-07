// compile: g++ -std=c++20 -O2 historical.cpp -o historical
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <sstream>
#include <ranges>
#include <iterator>

// ---- 1. 传统C风格 (危险、易错) ----
void c_style() {
    const char* input = "hello c plus plus world hello";
    // 模拟strtok: 危险, 修改输入字符串, 非线程安全
    char buf[256];
    strncpy(buf, input, sizeof(buf));
    buf[sizeof(buf)-1] = '\0';
    char* token = strtok(buf, " ");
    const char* words[20] = {0};
    int cnt = 0;
    while (token && cnt < 20) {
        words[cnt++] = token;
        token = strtok(nullptr, " ");
    }
    // 简陋的去重和排序，需大量手工代码...
    std::cout << "[C style] Not shown due to complexity.\n";
}

// ---- 2. C++98/03 风格 (使用标准库) ----
void cpp98_style() {
    std::string input = "hello c plus plus world hello";
    std::istringstream iss(input);
    std::vector<std::string> words;
    std::copy(std::istream_iterator<std::string>(iss),
              std::istream_iterator<std::string>(),
              std::back_inserter(words));
    std::sort(words.begin(), words.end());
    words.erase(std::unique(words.begin(), words.end()), words.end());
    std::cout << "[C++98] ";
    for (const auto& w : words) std::cout << w << " ";
    std::cout << "\n";
}

// ---- 3. C++20 范围库 (管道抽象) ----
void cpp20_ranges() {
    std::string input = "hello c plus plus world hello";
    auto view = std::ranges::views::split(std::string_view(input), ' ')
                | std::views::transform([](auto&& r) {
                    return std::string_view(&*r.begin(), std::ranges::distance(r));
                  })
                | std::views::filter([](std::string_view sv) { return !sv.empty(); })
                | std::ranges::to<std::vector<std::string>>(); // C++23, 可用 <algorithm> + sort + unique 备用

    // 兼容 C++20 的简洁去重排序 (仍需手动处理)
    auto words = std::move(view); // 假定 view 可转换为 vector
    std::ranges::sort(words);
    auto [first, last] = std::ranges::unique(words);
    words.erase(first, last);
    std::cout << "[C++20] ";
    for (auto& w : words) std::cout << w << " ";
    std::cout << "\n";
}

int main() {
    c_style();
    cpp98_style();
    cpp20_ranges();
    return 0;
}