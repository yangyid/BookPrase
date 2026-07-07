#include <iostream>
   #include <memory>
   #include <vector>
   #include <chrono>
   using namespace std;

   // 可移动/可拷贝的简单 buffer 类
   class Buffer {
       size_t size_;
       char* data_;
   public:
       explicit Buffer(size_t sz) : size_(sz), data_(new char[sz]) {
           cout << "[构造] 分配 " << size_ << " 字节\n";
       }
       // 拷贝构造（深拷贝）
       Buffer(const Buffer& other) : size_(other.size_), data_(new char[other.size_]) {
           copy(other.data_, other.data_ + other.size_, data_);
           cout << "[拷贝构造] 深拷贝 " << size_ << " 字节\n";
       }
       // 移动构造（资源转移）
       Buffer(Buffer&& other) noexcept : size_(other.size_), data_(other.data_) {
           other.size_ = 0;
           other.data_ = nullptr;
           cout << "[移动构造] 窃取资源\n";
       }
       ~Buffer() {
           cout << "[析构] ";
           if (data_) cout << "释放 " << size_ << " 字节\n";
           else        cout << "空壳（资源已移走）\n";
           delete[] data_;
       }
       // 避免拷贝赋值和移动赋值，仅演示核心
       Buffer& operator=(const Buffer&) = delete;
       Buffer& operator=(Buffer&&) = delete;
   };

   // 使用 lambda 和 auto
   int main() {
       // auto & 范围 for
       vector<int> vec = {1,2,3,4,5};
       cout << "范围 for + auto: ";
       for (auto v : vec) cout << v << " ";
       cout << endl;

       // nullptr
       int* p = nullptr;
       if (p == nullptr) cout << "p 是 nullptr\n";

       // 静态断言
       static_assert(sizeof(void*) >= 4, "平台指针至少 4 字节");

       // 智能指针+移动语义
       auto buf1 = make_unique<Buffer>(1024);
       // 转移所有权
       auto buf2 = move(buf1);  // 触发移动构造
       cout << "buf1 现在是空？" << (buf1 == nullptr ? "是" : "否") << endl;

       // lambda 捕获
       int counter = 10;
       auto lambda = [&counter]() { return ++counter; };
       cout << "lambda 调用后 counter = " << lambda() << endl;
   }