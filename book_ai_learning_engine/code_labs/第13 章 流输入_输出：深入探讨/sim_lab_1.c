#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <cassert>

// 用户自定义类型：二维点
struct Point {
    double x, y;
    Point(double x_ = 0, double y_ = 0) : x(x_), y(y_) {}
};

// 重载插入运算符
std::ostream& operator<<(std::ostream& os, const Point& p) {
    os << '(' << p.x << ", " << p.y << ')';
    return os;
}

// 重载提取运算符
std::istream& operator>>(std::istream& is, Point& p) {
    char ch;
    // 期望格式: (x,y)
    is >> ch;                 // 读取 '('
    if (ch != '(') {
        is.setstate(std::ios::failbit);
        return is;
    }
    is >> p.x >> ch >> p.y >> ch; // 读 x , y )
    if (ch != ')') is.setstate(std::ios::failbit);
    return is;
}

int main() {
    // 1. 格式化输出 + 状态检查
    double pi = 3.1415926535;
    std::cout << "Default: " << pi << '\n';
    std::cout << "Fixed prec 3: " << std::fixed << std::setprecision(3) << pi << '\n';
    std::cout << "Scientific: " << std::scientific << pi << '\n';
    std::cout << "Width 10 with fill: " << std::setw(10) << std::setfill('*') << std::right << 42 << '\n';

    // 2. 输入错误处理与恢复
    int number;
    std::cout << "Enter an integer: ";
    while (!(std::cin >> number)) {
        std::cout << "Invalid input. Please enter an integer: ";
        std::cin.clear();               // 清除错误状态
        std::cin.ignore(1000, '\n');    // 丢弃错误输入行
    }
    std::cout << "You entered: " << number << '\n';

    // 3. 文件写入与读取
    const std::string filename = "points.txt";
    {
        std::ofstream out(filename);
        if (!out) {
            std::cerr << "Cannot open file for writing\n";
            return 1;
        }
        std::vector<Point> pts = { {1.1, 2.2}, {3.3, 4.4} };
        for (const auto& p : pts) out << p << '\n';
    } // out 自动关闭

    {
        std::ifstream in(filename);
        if (!in) {
            std::cerr << "Cannot open file for reading\n";
            return 1;
        }
        std::vector<Point> loaded;
        Point temp;
        while (in >> temp) {
            loaded.push_back(temp);
        }
        // 展示读取结果
        for (const auto& p : loaded)
            std::cout << "Read point: " << p << '\n';
        assert(loaded.size() == 2);
    }

    // 4. 字符串流（内存格式化）
    std::ostringstream oss;
    oss << "Point " << Point(5.6, 7.8) << " has distance " 
        << std::sqrt(5.6*5.6 + 7.8*7.8);
    std::string report = oss.str();
    std::cout << "Generated string: " << report << '\n';

    // 5. 二进制读写示例
    {
        std::ofstream bf("data.bin", std::ios::binary);
        double arr[] = {1.23, 4.56, 7.89};
        bf.write(reinterpret_cast<const char*>(arr), sizeof(arr));
        bf.close();

        std::ifstream bi("data.bin", std::ios::binary);
        double in_arr[3];
        bi.read(reinterpret_cast<char*>(in_arr), sizeof(in_arr));
        std::cout << "Binary read: " << in_arr[0] << ' ' << in_arr[1] << ' ' << in_arr[2] << '\n';
    }

    return 0;
}