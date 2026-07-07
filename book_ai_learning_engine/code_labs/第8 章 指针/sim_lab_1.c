#include <iostream>
#include <memory>   // 智能指针
#include <vector>

int main() {
    // 1. 基本指针与nullptr (C++11)
    int value = 42;
    int* ptr = &value;
    std::cout << "Address: " << ptr << ", Value: " << *ptr << '\n';

    int* nullPtr = nullptr;   // 取代 NULL
    if (!nullPtr) std::cout << "nullPtr is nullptr\n";

    // 2. 动态内存与智能指针
    // unique_ptr: 独占所有权
    std::unique_ptr<int> uniqPtr(new int(100));
    std::cout << "unique_ptr value: " << *uniqPtr << '\n';
    // 转移所有权
    auto uniqPtr2 = std::move(uniqPtr);
    if (!uniqPtr) std::cout << "uniqPtr now empty\n";
    // C++14 make_unique, 这里使用C++11写法

    // shared_ptr: 共享所有权
    std::shared_ptr<std::vector<int>> sharedVec = 
        std::make_shared<std::vector<int>>(std::initializer_list<int>{1,2,3});
    std::shared_ptr<std::vector<int>> sharedVec2 = sharedVec;  // 引用计数+1
    std::cout << "shared_ptr use count: " << sharedVec.use_count() << '\n';

    // 3. 指针与数组
    int arr[3] = {10,20,30};
    int* pArr = arr;                     // 指向数组首元素
    for (int i = 0; i < 3; ++i) 
        std::cout << *(pArr + i) << ' '; // 指针算术
    std::cout << '\n';

    return 0;
} // uniqPtr2, sharedVec等自动释放内存，无泄漏