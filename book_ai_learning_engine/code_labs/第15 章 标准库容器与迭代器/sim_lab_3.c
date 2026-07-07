#include <unordered_map>
#include <chrono>
#include <iostream>

int main() {
    const int N = 1000000;
    std::unordered_map<int, int> umap;
    umap.reserve(N); // 预留桶数

    auto start = std::chrono::steady_clock::now();
    for (int i = 0; i < N; ++i)
        umap.emplace(i, i);
    auto end = std::chrono::steady_clock::now();

    std::cout << "Insertion time: " 
              << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count()
              << " ms\n";
    std::cout << "Bucket count: " << umap.bucket_count() 
              << ", load factor: " << umap.load_factor() << '\n';
    return 0;
}