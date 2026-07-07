#include <iostream>
   #include <iomanip>

   int main() {
       // 1. if-else 示例：成绩等级
       int score;
       std::cout << "Enter numeric score (0-100): ";
       std::cin >> score;

       if (score >= 90)
           std::cout << "Grade: A\n";
       else if (score >= 80)
           std::cout << "Grade: B\n";
       else if (score >= 70)
           std::cout << "Grade: C\n";
       else if (score >= 60)
           std::cout << "Grade: D\n";
       else
           std::cout << "Grade: F\n";

       // 2. while 循环：累加 1 到 n
       int n = 10, sum = 0, i = 1;
       while (i <= n) {
           sum += i;    // 复合赋值
           ++i;         // 前缀自增
       }
       std::cout << "Sum of 1 to " << n << " is " << sum << "\n";

       // 3. 体现前缀与后缀差异
       int a = 5, b = 5;
       int prefix = ++a;   // a变为6, prefix为6
       int postfix = b++;  // postfix为5, b变为6
       std::cout << "prefix: " << prefix << ", a: " << a << "\n";
       std::cout << "postfix: " << postfix << ", b: " << b << "\n";

       return 0;
   }