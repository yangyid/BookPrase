#include <iostream>
#include <version>   // C++20: 集中声明库特性宏

int main() {
    std::cout << "=== C++ Feature Test Report ===\n";

    // ----- 语言核心特性 -----
    #ifdef __cpp_concepts
        std::cout << "Concepts:           supported (value " << __cpp_concepts << ")\n";
    #else
        std::cout << "Concepts:           NOT supported\n";
    #endif

    #ifdef __cpp_impl_coroutine
        std::cout << "Coroutines (TS):    supported (value " << __cpp_impl_coroutine << ")\n";
    #elif defined(__cpp_coroutines)
        std::cout << "Coroutines (C++20): supported (value " << __cpp_coroutines << ")\n";
    #else
        std::cout << "Coroutines:         NOT supported\n";
    #endif

    #ifdef __cpp_impl_three_way_comparison
        std::cout << "Three-way comparison: supported (value " << __cpp_impl_three_way_comparison << ")\n";
    #else
        std::cout << "Three-way comparison: NOT supported\n";
    #endif

    // ----- 库特性 -----
    #ifdef __cpp_lib_ranges
        std::cout << "std::ranges:        supported (value " << __cpp_lib_ranges << ")\n";
    #else
        std::cout << "std::ranges:        NOT supported\n";
    #endif

    #ifdef __cpp_lib_span
        std::cout << "std::span:          supported (value " << __cpp_lib_span << ")\n";
    #else
        std::cout << "std::span:          NOT supported\n";
    #endif

    #ifdef __cpp_lib_to_array
        std::cout << "std::to_array:      supported (value " << __cpp_lib_to_array << ")\n";
    #else
        std::cout << "std::to_array:      NOT supported\n";
    #endif

    // 利用 static_assert 强制要求某个特性
    #ifdef __cpp_lib_bit_cast
        static_assert(__cpp_lib_bit_cast >= 201806L, "bit_cast implementation too old");
        std::cout << "std::bit_cast:      passed static_assert (value " << __cpp_lib_bit_cast << ")\n";
    #else
        std::cout << "std::bit_cast:      NOT supported\n";
    #endif

    return 0;
}