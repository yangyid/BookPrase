## 第7章 类模板 array 和 vector；捕获异常

- `std::array` 是定长、栈分配的容器，封装了 C 风格数组，提供标准容器接口（如 `size()`、`at()`、迭代器），无额外时空开销，但大小必须在编译期确定。
- `std::vector` 是动态、堆分配的容器，支持运行时增删元素，自动管理内存（通常以 2 倍或 1.5 倍扩容），是替代 `new[]`/`delete[]` 的首选。
- 异常处理 `try`/`catch`/`throw` 用于分离正常逻辑与错误处理，`noexcept` 声明可优化代码，防止异常穿透栈帧。
- 容器元素访问：`[]` 不检查越界（未定义行为），`at()` 越界时抛出 `std::out_of_range` 异常，两者构成性能与安全的权衡。

### 1. 核心理论断言，白话类比与大师避坑指南
**断言**  
`vector` 为“动态数组”，`array` 为“封装版静态数组”。  
`vector` 的容量增长采用摊销常数时间：当 `size() > capacity()` 时重新分配更大的内存块并搬移/拷贝元素，扩容因子通常为 2 或 1.5，确保插入操作的均摊成本为 O(1)。  
`array` 的大小是类型的一部分：`array<int, 5>` 和 `array<int, 10>` 是不同的类型，不能直接赋值或隐式转换。  
异常捕获时，应优先按引用捕获（避免对象切片），并提供 `noexcept` 保证给移动构造、析构、`swap` 等关键函数，以提升性能和安全性。

**白话类比**  
- `array` 就像一栋固定房间数的房子，房间数写在房产证上，不能多也不能少。  
- `vector` 像会自动扩建的活动板房，当入住的人超过现有房间数，就重新找一块更大的空地，把原有住户搬过去，然后释放旧板房。  
- `at()` 就像有门禁卡验证，没卡强行闯入会报警（异常）；`operator[]` 则是没有门禁，闯入可能直接导致系统崩溃（未定义行为）。  
- `noexcept` 告诉编译器：“这个函数从不抛异常，你尽可放心优化，不必准备异常处理的桩子。”

**大师避坑指南（Scott Meyers / Bjarne Stroustrup）**  
- 尽量用 `std::vector` 和 `std::array` 替代原生数组；原生数组会退化为指针丢失大小信息，且不支持范围 for、标准算法。  
- 捕获异常时不要 `catch(...)` 后悄悄吞掉异常，除非在析构函数中记录并传播或终止。  
- 编写移动构造函数和移动赋值运算符时，标记为 `noexcept`；否则标准库（如 `vector`）会在重新分配时使用拷贝而非移动，失去性能优势。  
- `vector<bool>` 是特化的位压缩容器，不是真正的 `bool` 容器，其引用代理对象的行为不同于普通 `bool&`，慎用。  
- 当容器中存储资源管理对象（如智能指针）时，注意异常安全：在修改容器前先完成可能抛出异常的操作，或利用临时对象和 `swap`。

### 2. 保姆级可编译运行代码
```cpp
#include <iostream>
#include <array>
#include <vector>
#include <stdexcept>
#include <string>

// 自定义异常类
class ContainerException : public std::runtime_error {
public:
    explicit ContainerException(const std::string& msg)
        : std::runtime_error("ContainerException: " + msg) {}
};

// 一个有异常安全保护的函数：向 vector 添加元素
void safeAdd(std::vector<int>& vec, int val) {
    if (val < 0) {
        throw ContainerException("Negative value not allowed");
    }
    vec.push_back(val);
}

int main() {
    // ---- std::array 演示 ----
    std::array<int, 4> arr = {1, 2, 3, 4};
    std::cout << "array size: " << arr.size() << '\n';
    try {
        std::cout << "arr.at(4) = " << arr.at(4) << '\n';  // 越界，抛出 std::out_of_range
    } catch (const std::out_of_range& e) {
        std::cerr << "Caught out_of_range: " << e.what() << '\n';
    }
    // 不检查越界，未定义行为（注释掉以避免崩溃）
    // std::cout << "arr[4] = " << arr[4] << '\n';

    // ---- std::vector 演示 ----
    std::vector<int> vec{10, 20, 30};
    std::cout << "vector size: " << vec.size() << ", capacity: " << vec.capacity() << '\n';
    vec.push_back(40);
    std::cout << "After push_back, size: " << vec.size() << ", capacity: " << vec.capacity() << '\n';

    // 调用可能抛出自定义异常的函数
    try {
        safeAdd(vec, -5);
    } catch (const ContainerException& e) {
        std::cerr << e.what() << '\n';
    } catch (const std::exception& e) {
        std::cerr << "Standard exception: " << e.what() << '\n';
    }

    // 使用 at() 安全访问
    try {
        std::cout << "vec.at(10) = " << vec.at(10) << '\n';
    } catch (const std::out_of_range& e) {
        std::cerr << "Out of range access prevented: " << e.what() << '\n';
    }

    return 0;
}
```

### 3. 内核/系统验证实验
**实验目标**：观察 `vector` 扩容行为、内存重分配以及 `noexcept` 对元素移动的影响。

**实验步骤**：
1. 使用如下代码，通过自定义析构/构造监控 `vector` 扩容时元素的复制/移动次数。
```cpp
#include <iostream>
#include <vector>

struct Tracker {
    static inline int copies = 0, moves = 0;
    int id;
    Tracker(int i) : id(i) {}
    Tracker(const Tracker& o) : id(o.id) { ++copies; }
    Tracker(Tracker&& o) noexcept : id(o.id) { ++moves; }
};

int main() {
    std::vector<Tracker> v;
    v.reserve(2);   // 预留容量，避免早期扩容干扰
    std::cout << "--- Adding elements ---\n";
    for (int i = 0; i < 5; ++i) {
        v.emplace_back(i);
        std::cout << "size: " << v.size() << " capacity: " << v.capacity()
                  << " copies: " << Tracker::copies << " moves: " << Tracker::moves << '\n';
    }
    // 尝试将移动构造的 noexcept 去掉，观察拷贝是否增加
}
```
- 若移动构造函数不带 `noexcept`，`vector` 重新分配时会调用拷贝构造而非移动构造，拷贝次数大增。  
- 使用 `ltrace` 或 `strace`（Linux）可追踪 `malloc`/`free` 调用，证实扩容时的内存分配。  
- 使用 GDB 断点在 `__throw_out_of_range` 上，观察 `at()` 越界触发异常时的堆栈展开。

**实验结论**：标准容器依赖 `noexcept` 保证以采用更高效的移动语义；异常处理有成本，但 `at()` 只在真正越界时才抛出，平衡了安全与性能。

### 4. 理论-实践映射表
| 理论概念 | 对应实践 | 说明 |
|----------|----------|------|
| 栈分配、编译期大小 | `std::array<int, 10>` 内存布局与 `int[10]` 一致，声明于函数内即占栈空间 | 可用 `sizeof` 验证 |
| 堆分配、动态扩容 | `std::vector<int>`，`push_back` 触发 `realloc` | `vector` 内部指针指向堆 |
| 均摊 O(1) 插入 | 扩容因子 2 导致复制元素总次数与新元素数量呈线性 | 通过计数器验证 |
| 类型安全访问 | `at()` vs `operator[]` | 越界时前者抛出 `out_of_range`，后者未定义行为 |
| 异常捕获最佳实践 | `catch(const std::exception& e)` 按引用捕获 | 避免对象切片，保证多态 |
| 异常安全保证 | 先完成可能抛出异常的临时操作，再用 `swap` 或 `noexcept` 的移动赋值修改容器 | 实现“强保证” |
| `noexcept` 性能优化 | 移动构造加 `noexcept` 使 `vector` 重分配时使用移动 | 减少临时对象深度拷贝 |
| `vector<bool>` 陷阱 | `vector<bool> vb(10); auto b = vb[0];` 得到的是代理引用，非 `bool&` | 标准建议使用 `deque<bool>` 或 `vector<char>` 替代 |

### 5. 可深挖的知识点与学习链接
- **C++ 参考手册**：[cppreference.com – std::array](https://en.cppreference.com/w/cpp/container/array)，[std::vector](https://en.cppreference.com/w/cpp/container/vector)，[异常](https://en.cppreference.com/w/cpp/language/throw)  
- **扩容策略分析**：《Effective STL》条款 14（使用 `reserve` 避免不必要的重新分配）；gcc/libstdc++ 扩容因子为 2，MSVC 为 1.5，见 `_M_check_len`。  
- **异常安全**：Herb Sutter 《Exceptional C++》 关于强保证、基本保证、不抛出保证；Scott Meyers 《Effective Modern C++》条款 15：只要有可能就使用 `constexpr`，条款 17 理解特殊成员函数生成，`noexcept` 影响。  
- **`vector<bool>` 特化陷阱**：cppreference 明确“它不满足容器或可逆容器要求”，推荐替代方案。  
- **自定义分配器**：可配合 `std::vector` 的模板参数实现内存池或跟踪分配（学习链接：cppreference `std::allocator`）。  
- **C++17 的 `std::size()` 与范围 for 改进**：`std::size(arr)` 通用于 `array` 和 C 数组。

### 6. 代码练习任务
1. **安全向量包装**  
   编写一个类模板 `SafeVector<T>`，内部使用 `std::vector<T>`，并且重载 `operator[]` 执行边界检查（当索引越界时抛出 `std::out_of_range`，类似 `at()`），同时提供 `at()` 直接委托内部实现。添加 `push_back` 时检查元素合法性（例如不能为布尔假值？自选条件，抛出自定义异常）。

2. **观察扩容与性能**  
   编写程序向 `vector<int>` 添加 10000 个随机整数，并打印每次扩容时的新容量。用 `reserve` 预先分配足够空间，对比两种版本的运行时间（可用 `<chrono>`）。总结 `reserve` 的使用场景。

3. **异常安全练习**  
   设计一个类 `DataBase`，管理一个 `std::vector<std::string>`，要求提供成员函数 `replaceAll(const std::string& old, const std::string& new)`，将容器中所有等于 `old` 的串替换为 `new`。要求该函数实现强异常安全保证：如果替换过程中发生异常（例如 `new` 可能抛出 `bad_alloc`），容器状态不变。提示：可使用临时 `vector` + `swap`。

4. **自定异常层次**  
   创建一个异常体系：基类 `MyException` 继承自 `std::runtime_error`，派生类 `FileOpenException`、`ParseErrorException` 等。编写一个函数模拟文件解析，可能抛出不同异常，在 `main` 中用依次捕获特定异常和通用 `MyException` 来展示捕获顺序和多态。

5. **`array` 与 C 接口交互**  
   实现一个函数 `void process(const std::array<double, 4>& data)`，并将其封装成一个 C 兼容的接口 `c_process(double* data, int size)`，内部调用 `process` 前检查大小是否为 4。重点展示 `array` 的 `data()` 成员函数，确保没有数组退化危险。  
   额外：在 C++ 代码侧测试传递 `std::array` 与原生数组给 `c_process` 的情形。

