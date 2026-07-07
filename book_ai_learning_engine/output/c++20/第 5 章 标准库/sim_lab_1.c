#include <format>
   #include <span>
   #include <ranges>
   #include <vector>
   #include <chrono>
   #include <bit>
   #include <source_location>
   #include <syncstream>
   #include <iostream>
   #include <cassert>

   void log(std::string_view msg,
            std::source_location loc = std::source_location::current()) {
       std::osyncstream(std::cout)
           << std::format("{}:{} in {}: {}",
                          loc.file_name(), loc.line(), loc.function_name(), msg)
           << '\n';
   }

   int main() {
       // 1. 格式化：类型安全、编译期检查
       std::string s = std::format("Hello, {}! The answer is {:.2f}", "C++20", 42.0);
       assert(s == "Hello, C++20! The answer is 42.00");

       // 2. span：统一视图，无拷贝
       int raw[] = {1,2,3,4,5};
       std::span<int> sp(raw);
       std::vector<int> v = {6,7,8};
       std::span<int> spv(v);
       assert(sp.size() == 5 && spv.size() == 3);
       sp[0] = 100; // 影响 raw[0]
       assert(raw[0] == 100);

       // 3. 范围适配器 + 管道
       namespace rv = std::ranges::views;
       auto even_squares = v | rv::filter([](int i){ return i % 2 == 0; })
                              | rv::transform([](int i){ return i * i; });
       // 惰性求值，直到迭代才计算
       std::vector<int> result;
       std::ranges::copy(even_squares, std::back_inserter(result));
       assert((result == std::vector{36, 64})); // 6*6, 8*8

       // 4. 日历与时间
       using namespace std::chrono;
       auto today = year_month_day{floor<days>(system_clock::now())};
       auto next_week = sys_days{today} + weeks{1};
       auto next_week_ymd = year_month_day{next_week};
       std::string date_str = std::format("{:%F}", next_week);
       log(std::format("Today: {:%F}, one week later: {}", today, date_str));

       // 5. bit_cast
       float f = 3.14f;
       auto bits = std::bit_cast<uint32_t>(f);
       float f2 = std::bit_cast<float>(bits);
       assert(f == f2);

       return 0;
   }