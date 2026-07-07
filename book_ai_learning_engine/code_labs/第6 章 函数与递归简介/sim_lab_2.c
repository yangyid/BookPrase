// deep_recursion.cpp
   #include <iostream>
   void recurse(int n) {
       int a[1000]{};        // 消耗栈帧
       if (n == 0) return;
       std::cout << "depth " << n << ", addr of local a: " << &a << '\n';
       recurse(n-1);
   }
   int main() {
       recurse(100000);      // 可能溢出
   }