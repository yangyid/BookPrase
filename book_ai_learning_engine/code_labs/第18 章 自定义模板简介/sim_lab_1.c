#include <iostream>
   #include <vector>
   #include <type_traits>
   // 函数模板：交换任意类型的两个值
   template<typename T>
   void mySwap(T& a, T& b) {
       T temp = a;
       a = b;
       b = temp;
   }
   // 类模板：简单的栈
   template<typename ElemType = int, size_t MaxSize = 10>
   class Stack {
       ElemType data[MaxSize];
       size_t top = 0;
   public:
       bool push(const ElemType& val) {
           if (top >= MaxSize) return false;
           data[top++] = val;
           return true;
       }
       bool pop(ElemType& out) {
           if (top == 0) return false;
           out = data[--top];
           return true;
       }
       // 成员函数模板：打印栈中元素（接受不同输出流）
       template<typename Stream>
       void print(Stream& os) const {
           for (size_t i = 0; i < top; ++i)
               os << data[i] << " ";
           os << std::endl;
       }
   };
   // 全特化：针对指针类型的 Stack 特化版本
   template<size_t MaxSize>
   class Stack<const char*, MaxSize> {
       const char* data[MaxSize];
       size_t top = 0;
   public:
       bool push(const char* val) {
           if (top >= MaxSize) return false;
           data[top++] = val;
           return true;
       }
       bool pop(const char*& out) {
           if (top == 0) return false;
           out = data[--top];
           return true;
       }
   };
   int main() {
       // 函数模板使用
       int a = 5, b = 10;
       mySwap(a, b);
       std::cout << a << " " << b << std::endl; // 10 5
       // 类模板使用（默认参数：int 类型，最大10个元素）
       Stack<> intStack;
       intStack.push(1);
       intStack.push(2);
       int val;
       intStack.pop(val);
       std::cout << "Popped: " << val << std::endl; // 2
       // 特化使用
       Stack<const char*, 20> strStack;
       strStack.push("hello");
       const char* s;
       strStack.pop(s);
       std::cout << "Popped string: " << s << std::endl;
       return 0;
   }