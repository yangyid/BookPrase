#include <iostream>
#include <stdexcept>
#include <memory>

class Resource {
public:
    Resource(int id) : id_(id) { std::cout << "Resource " << id_ << " acquired.\n"; }
    ~Resource() { std::cout << "Resource " << id_ << " released.\n"; }
private:
    int id_;
};

void risky_func(int level) {
    Resource res(level);
    if (level == 0) throw std::runtime_error("level 0: critical error");
    if (level == 1) throw std::logic_error("level 1: invalid state");
    std::cout << "risky_func completed.\n";
}

// noexcept承诺不抛出异常，若内部抛出会调用std::terminate
void safe_func() noexcept {
    Resource res(99);
    std::cout << "safe_func: guaranteed no throw.\n";
}

int main() {
    try {
        risky_func(0);
    } catch (const std::runtime_error& e) {
        std::cerr << "Caught runtime_error: " << e.what() << '\n';
    } catch (const std::logic_error& e) {
        std::cerr << "Caught logic_error: " << e.what() << '\n';
    } catch (...) {
        std::cerr << "Caught unknown exception.\n";
    }

    std::cout << "Calling noexcept function:\n";
    safe_func();

    return 0;
}