#include <iostream>
#include <vector>
#include <list>
#include <map>
#include <unordered_map>
#include <string>
#include <algorithm>
#include <iterator>

int main() {
    // ---------- vector ----------
    std::vector<int> vec {1, 2, 3, 4, 5};
    vec.push_back(6);
    vec.emplace_back(7);  // 原位构造

    std::cout << "vector elements: ";
    for (const auto& v : vec) std::cout << v << ' ';
    std::cout << '\n';

    // 使用迭代器删除偶数
    for (auto it = vec.begin(); it != vec.end(); ) {
        if (*it % 2 == 0)
            it = vec.erase(it); // erase返回下一个有效迭代器
        else
            ++it;
    }

    std::cout << "After removing evens: ";
    for (auto it = vec.cbegin(); it != vec.cend(); ++it)
        std::cout << *it << ' ';
    std::cout << '\n';

    // ---------- list ----------
    std::list<std::string> words {"hello", "world", "c++", "11"};
    words.emplace_front("start");
    words.emplace_back("end");

    // list 支持双向迭代器
    std::cout << "list: ";
    for (auto it = words.begin(); it != words.end(); ++it)
        std::cout << *it << ' ';
    std::cout << '\n';

    // ---------- map (有序) ----------
    std::map<std::string, int> age_map {{"Alice", 30}, {"Bob", 25}};
    age_map.emplace("Charlie", 28);
    age_map["Diana"] = 22;  // 注意: 若不存在则插入

    std::cout << "map contents:\n";
    for (const auto& pair : age_map)
        std::cout << pair.first << " is " << pair.second << " years old.\n";

    // 查找
    auto it = age_map.find("Bob");
    if (it != age_map.end())
        std::cout << "Found Bob, age = " << it->second << '\n';

    // ---------- unordered_map ----------
    std::unordered_map<std::string, double> umap {
        {"pi", 3.1415}, {"e", 2.7182}, {"phi", 1.618}
    };
    umap.emplace("sqrt2", 1.4142);

    std::cout << "unordered_map:\n";
    for (const auto& p : umap)  // 顺序不确定
        std::cout << p.first << " -> " << p.second << '\n';

    // 用at安全访问
    try {
        std::cout << "pi = " << umap.at("pi") << '\n';
        std::cout << "noexist = " << umap.at("noexist") << '\n'; // 抛出异常
    } catch (const std::out_of_range& e) {
        std::cerr << "Exception: " << e.what() << '\n';
    }

    return 0;
}