#include <mdspan>
#include <vector>
constexpr int test_mdspan() {
    std::vector v = {1,2,3,4,5,6};
    // C++23 支持的多维下标（通过 operator[] 多参数）
    std::mdspan<int, std::extents<int,2,3>> m{v.data()};
    return m[1,2]; // 访问第1行第2列，期望值6
}
static_assert(test_mdspan() == 6);