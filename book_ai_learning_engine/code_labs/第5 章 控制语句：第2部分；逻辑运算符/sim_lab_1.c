#include <iostream>
#include <cctype> // for isdigit
using namespace std;

int main() {
    // 1. 短路求值演示：安全解引用指针
    int value = 42;
    int* ptr = nullptr;

    if (ptr != nullptr && *ptr > 0) {
        cout << "Unreachable due to short circuit." << endl;
    } else {
        cout << "Pointer is null, short circuit avoids crash." << endl;
    }

    // 2. 逻辑运算符结合关系判断闰年
    int year;
    cout << "Enter year: ";
    cin >> year;
    bool isLeap = (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
    cout << year << " is " << (isLeap ? "" : "not ") << "a leap year." << endl;

    // 3. 输入校验：要求输入非负数字字符
    char ch;
    cout << "Enter a digit (0-9): ";
    cin >> ch;
    if (ch >= '0' && ch <= '9') {
        cout << "Valid digit: " << ch << endl;
    } else {
        cout << "Not a digit!" << endl;
    }

    return 0;
}