#include <iostream>
   #include <coroutine>
   #include <thread>
   #include <chrono>
   #include <queue>
   #include <functional>
   #include <cassert>

   // 简易单线程调度器
   std::queue<std::function<void()>> task_queue;

   // 基础 Task 协程类型
   struct Task {
       struct promise_type {
           Task get_return_object() {
               return Task{std::coroutine_handle<promise_type>::from_promise(*this)};
           }
           std::suspend_never initial_suspend() noexcept { return {}; } // 立即启动
           std::suspend_always final_suspend() noexcept {            // 结束时挂起，等待外部销毁
               return {};
           }
           void return_void() noexcept {}
           void unhandled_exception() { std::terminate(); }
       };

       std::coroutine_handle<promise_type> handle;
       explicit Task(std::coroutine_handle<promise_type> h) : handle(h) {}
       ~Task() { if (handle) handle.destroy(); }
   };

   // 可暂停的异步操作：模拟 I/O 等待
   struct AsyncRead {
       bool await_ready() noexcept { return false; } // 总是需要挂起
       void await_suspend(std::coroutine_handle<> h) noexcept {
           // 将协程句柄提交给调度器，1 秒后恢复
           task_queue.push([h]() mutable {
               std::this_thread::sleep_for(std::chrono::seconds(1));
               h.resume();
           });
       }
       void await_resume() noexcept { std::cout << "数据读取完成\n"; }
   };

   Task process_request() {
       std::cout << "开始异步读取...\n";
       co_await AsyncRead{};
       std::cout << "后续处理逻辑\n";
   }

   int main() {
       auto task = process_request(); // 协程立即运行至第一个 co_await
       while (!task_queue.empty()) {
           auto work = std::move(task_queue.front());
           task_queue.pop();
           work(); // 模拟单线程事件循环
       }
       std::cout << "主流程结束，task 将在此析构清理协程帧\n";
   }