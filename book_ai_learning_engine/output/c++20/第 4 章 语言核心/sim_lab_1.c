#include <iostream>
   #include <concepts>
   #include <compare>
   #include <vector>
   #include <ranges>
   #include <generator>  // C++23标准生成器，此处演示C++20协程需自行实现。
                         // 为简洁，用范围适配器展示ranges。
   
   // 1. 概念约束
   template<typename T>
   concept Addable = requires(T a, T b) {
       { a + b } -> std::same_as<T>;
   };
   
   template<Addable T>
   T add(T a, T b) { return a + b; }
   
   // 2. 三路比较
   struct Person {
       std::string name;
       int age;
       auto operator<=>(const Person&) const = default; // 按声明的成员顺序比较
   };
   
   int main() {
       // 概念测试
       std::cout << add(3, 4) << '\n';   // OK
       // std::cout << add("hello", "world"); // 编译错误：const char* 不满足Addable
   
       // 三路比较
       Person alice{"Alice", 30}, bob{"Bob", 25};
       if (alice > bob) std::cout << "Alice older\n";
   
       // 范围管道
       std::vector<int> nums = {1,2,3,4,5,6};
       auto result = nums 
                   | std::views::filter([](int n){ return n % 2 == 0; })
                   | std::views::transform([](int n){ return n * n; });
       for (int x : result) std::cout << x << ' '; // 4 16 36
       std::cout << '\n';
   }