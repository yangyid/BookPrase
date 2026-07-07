#include <iostream>
#include <string>
#include <sstream>
#include <cassert>

void demo_move() {
    std::string src = "Giant heavy payload that I don't want to copy";
    std::string dest = std::move(src);  // 移动赋值
    std::cout << "After move:\n src = \"" << src << "\" (empty)\n";
    std::cout << " dest = \"" << dest << "\"\n";
}

void demo_stream() {
    std::istringstream iss("42 3.14 Hello");
    int i; double d; std::string w;
    if (iss >> i >> d >> w)
        std::cout << "Parsed: " << i << ", " << d << ", " << w << '\n';
    
    std::ostringstream oss;
    oss << "Formatted: " << i << '+' << d << " = " << (i+d);
    std::string result = oss.str();   // 得到格式化字符串
    std::cout << result << '\n';
}

void demo_sso_address_change() {
    std::string s = "short";
    const char* addr1 = s.data();
    s += " -> now this string exceeds typical SSO boundary ................";
    const char* addr2 = s.data();
    std::cout << "SSO address changed? " << (addr1 != addr2 ? "YES" : "NO") << '\n';
}

void demo_stoi() {
    std::string num = "   -123abc";
    std::size_t pos;
    int val = std::stoi(num, &pos);
    std::cout << "Converted: " << val << ", first unconverted at index " << pos << '\n';
}

int main() {
    demo_move();
    demo_stream();
    demo_sso_address_change();
    demo_stoi();
    return 0;
}