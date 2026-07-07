#include <iostream>
#include <vector>

// 递归阶乘，演示基例与递归调用
unsigned long long factorial(unsigned int n) {
    if (n <= 1) return 1;               // 基例
    return n * factorial(n - 1);        // 递归分解
}

// 重载：乘上系数 k 的阶乘（默认系数为 2）
unsigned long long factorial(unsigned int n, unsigned int k) {
    if (n <= 1) return k;
    return n * factorial(n - 1, k);
}

// 引用传参交换两个数（避免值传递陷阱）
void swap(int& a, int& b) {
    int temp = a;
    a = b;
    b = temp;
}

// 计算 Fibonacci 数列（迭代版，避免递归树分裂）
std::vector<unsigned long long> fibonacci(unsigned int n) {
    if (n == 0) return {};
    std::vector<unsigned long long> fibs(n);
    fibs[0] = 1;
    if (n > 1) fibs[1] = 1;
    for (unsigned int i = 2; i < n; ++i)
        fibs[i] = fibs[i-1] + fibs[i-2];
    return fibs;
}

int main() {
    std::cout << "5! = " << factorial(5) << '\n';
    std::cout << "3! * 系数2 = " << factorial(3, 2) << '\n';

    int x = 10, y = 20;
    swap(x, y);
    std::cout << "swap 后: x=" << x << ", y=" << y << '\n';

    auto fib10 = fibonacci(10);
    std::cout << "前10项 Fibonacci:";
    for (auto v : fib10) std::cout << ' ' << v;
    std::cout << '\n';
    return 0;
}