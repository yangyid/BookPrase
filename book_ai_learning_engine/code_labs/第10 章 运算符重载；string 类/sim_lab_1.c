#include <iostream>
   #include <cstring>
   #include <algorithm>

   class MyString {
       char* data;
       size_t len;
   public:
       MyString() : data(new char[1]{'\0'}), len(0) {}
       MyString(const char* s) : data(nullptr), len(std::strlen(s)) {
           data = new char[len + 1];
           std::copy(s, s + len + 1, data);
       }
       MyString(const MyString& other) : data(new char[other.len + 1]), len(other.len) {
           std::copy(other.data, other.data + len + 1, data);
       }
       ~MyString() { delete[] data; }

       MyString& operator=(MyString other) {
           swap(*this, other);
           return *this;
       }
       friend void swap(MyString& a, MyString& b) noexcept {
           using std::swap;
           swap(a.data, b.data);
           swap(a.len, b.len);
       }

       // 重载 + 拼接 (非成员实现，但通过友元或成员均可，这里用成员)
       MyString operator+(const MyString& rhs) const {
           MyString result;
           result.len = len + rhs.len;
           result.data = new char[result.len + 1];
           std::copy(data, data + len, result.data);
           std::copy(rhs.data, rhs.data + rhs.len + 1, result.data + len);
           return result;
       }

       // 下标访问 (非常量版本)
       char& operator[](size_t index) {
           return data[index];
       }
       const char& operator[](size_t index) const {
           return data[index];
       }

       size_t size() const { return len; }
       const char* c_str() const { return data; }

       // 流插入必须为非成员函数，故声明为友元
       friend std::ostream& operator<<(std::ostream& os, const MyString& s);
   };

   std::ostream& operator<<(std::ostream& os, const MyString& s) {
       os << s.data;
       return os;
   }

   int main() {
       MyString s1("Hello");
       MyString s2(" World");
       MyString s3 = s1 + s2;   // 调用 operator+
       std::cout << s3 << std::endl;  // 输出 Hello World
       s3[0] = 'h';             // 调用非const operator[]
       std::cout << s3 << std::endl;  // 输出 hello World
       return 0;
   }