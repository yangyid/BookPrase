#include <iostream>
   int main() {
       std::cout << "Hello";      // 行缓冲，内容留在缓冲区，不立即 write
       std::cerr << "Error!";     // 无缓冲，立即 write
       std::cout << " World\n";   // 遇到 '\n' 触发行缓冲刷新，也会 write
       return 0;
   }