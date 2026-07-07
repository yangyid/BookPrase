#include <coroutine>
#include <iostream>
#include <optional>

template<typename T>
struct Generator {
    struct promise_type {
        std::optional<T> current;
        auto get_return_object() { return Generator{handle::from_promise(*this)}; }
        auto initial_suspend() { return std::suspend_always{}; }
        auto final_suspend() noexcept { return std::suspend_always{}; }
        auto yield_value(T val) { current = val; return std::suspend_always{}; }
        void return_void() {}
        void unhandled_exception() { std::terminate(); }
    };
    using handle = std::coroutine_handle<promise_type>;
    handle coro;
    Generator(handle h) : coro(h) {}
    ~Generator() { if (coro) coro.destroy(); }
    T operator()() {
        coro.resume();
        return *coro.promise().current;
    }
};

Generator<int> count(int start) {
    for (int i = start;; ++i) co_yield i;
}

int main() {
    auto gen = count(10);
    for (int k = 0; k < 5; ++k)
        std::cout << gen() << ' '; // 输出 10 11 12 13 14
}