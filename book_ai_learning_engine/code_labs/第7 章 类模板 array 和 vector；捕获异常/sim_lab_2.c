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