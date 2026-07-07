// 要求编译器支持C++23，如GCC13+ -std=c++2b
#include <expected>
#include <string>
#include <iostream>
#include <print>

std::expected<double, std::string> safe_sqrt(double x) {
    if (x < 0) return std::unexpected("Negative input");
    return std::sqrt(x);
}

struct Widget {
    int value;
    // C++23 显式对象参数：显式指定调用对象的传递方式
    void print(this Widget const& self) {
        std::println("const Widget: {}", self.value);
    }
    void set(this Widget& self, int v) {
        self.value = v; // self 是引用，直接修改
    }
};

int main() {
    // 测试 std::expected
    auto result = safe_sqrt(-4.0);
    if (result) std::println("sqrt: {}", *result);
    else std::println("Error: {}", result.error());

    // 显式对象参数：区分 const/非const 重载
    const Widget cw{10};
    cw.print(); // 调用 const& 重载
    Widget w{5};
    w.set(20);
    w.print();   // 依然调用 const& 重载，因为print的参数是const&，match
    // 如需非const版本可另行定义
}