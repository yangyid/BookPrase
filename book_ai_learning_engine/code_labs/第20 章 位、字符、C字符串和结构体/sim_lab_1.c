#include <iostream>
#include <cctype>
#include <cstring>
#include <iomanip>
#include <bitset>

// 位域结构体：硬件寄存器模拟
struct DeviceStatus {
    uint8_t power : 1;   // 1位
    uint8_t error : 1;
    uint8_t : 2;        // 未使用的2位
    uint8_t mode : 4;   // 4位
};

// 普通结构体，演示对齐与统一初始化
struct Packet {
    uint16_t id;
    uint8_t  flags;
    uint32_t length;    // 可能产生填充
};

int main() {
    // ---- 位操作 ----
    uint8_t reg = 0b10110010;
    uint8_t mask = 0b00001111;
    std::cout << "Original: " << std::bitset<8>(reg) << '\n';
    reg |= (1 << 3);                     // 置位
    std::cout << "Set bit3: " << std::bitset<8>(reg) << '\n';
    std::cout << "Lower nibble: " << std::bitset<8>(reg & mask) << '\n';

    // ---- 安全字符处理 ----
    char ch = 'é';  // 可能为负值（取决于字符编码和char符号性）
    // 正确做法：强制转换为 unsigned char
    if (std::isalpha(static_cast<unsigned char>(ch)))
        std::cout << "The character is alphabetic.\n";

    // ---- C 字符串（危险演示） ----
    char src[] = "Hello C-Style";
    char dest[10];
    // strcpy(dest, src);  // 缓冲区溢出！禁止使用
    std::strncpy(dest, src, sizeof(dest) - 1);
    dest[sizeof(dest) - 1] = '\0';  // 确保结尾
    std::cout << "Safely copied: " << dest << '\n';

    // ---- 结构体与C++11统一初始化 ----
    Packet p1{0x1234, 0xFF, 1024};           // 统一初始化
    Packet p2 = {0x5678, 0x01, 2048};        // 聚合初始化
    std::cout << "p1 length: " << p1.length << ", size of Packet: " << sizeof(Packet) << '\n';

    DeviceStatus status{};
    status.power = 1;
    status.mode = 5;
    std::cout << "Status byte: " << std::bitset<8>(*(reinterpret_cast<uint8_t*>(&status))) << '\n';

    return 0;
}