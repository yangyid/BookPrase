// demo02.cpp
#include <iostream>
#include <string>

int main() {
    // 基本输出
    std::cout << "Enter your name and age: ";
    
    std::string name;
    int age;
    // 链式输入
    std::cin >> name >> age;
    
    // 算术运算演示
    int sum = age + 10;
    double halfAge = age / 2.0;  // 浮点除法
    
    std::cout << "Hello, " << name 
              << "! In 10 years you will be " << sum 
              << " years old.\n";
    std::cout << "Half your age is " << halfAge << std::endl;
    
    // 关系与自增运算
    int a = 5, b = 5;
    std::cout << "a++: " << a++ << ", now a = " << a << "\n";   // 输出 5, a=6
    std::cout << "++b: " << ++b << ", now b = " << b << "\n";   // 输出 6, b=6
    std::cout << std::boolalpha << "a > b ? " << (a > b) << "\n"; // false
    
    return 0;
}