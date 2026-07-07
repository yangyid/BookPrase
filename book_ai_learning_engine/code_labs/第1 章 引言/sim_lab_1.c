// intro_demo.cpp -- 展示 C++11 引言级特性
   #include <iostream>
   #include <vector>
   #include <map>

   int main() {
       // 1. auto 类型推导
       auto meaning = 42;                // int
       auto pi = 3.14159;               // double
       auto greeting = std::string("C++11"); // std::string

       // 2. 统一的列表初始化
       std::vector<int> primes = {2, 3, 5, 7, 11};
       std::map<std::string, int> ages {
           {"Alice", 30},
           {"Bob", 25}
       };

       // 3. nullptr 安全指针
       int* ptr = nullptr;  // 不再是 #define NULL 0
       if (ptr == nullptr) {
           std::cout << "Pointer is safely null\n";
       }

       // 4. 基于范围的 for 循环 + auto
       for (const auto& p : primes) {
           std::cout << p << " ";
       }
       std::cout << std::endl;

       for (const auto& [name, age] : ages) { // C++17 结构化绑定，此处仅示意C++11用法：可分解pair
           std::cout << name << " is " << age << '\n';
       }

       return 0;
   }