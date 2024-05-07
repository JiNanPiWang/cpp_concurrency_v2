//
// Created by Samsara on 2024/5/6.
// File name: 4.cpp
// Description: 
//
#include <iostream>
#include <chrono>
#include <unistd.h>
#include <future>

using namespace std;

int main()
{
    auto start_time = std::chrono::system_clock::now();
    auto end_time = start_time + std::chrono::seconds(90);
    // 60s算一次，也就是计算多少分钟了
    auto duration = std::chrono::duration_cast<std::chrono::duration<int, std::ratio<60, 1>>>(end_time - start_time);
    cout << "duration with std::ratio<60, 1>: " << duration.count() << endl;

    auto max_time=30s;
    // 实现：
    /// Literal suffix for durations of type `std::chrono::seconds`
    // template <char... _Digits>
    // constexpr chrono::seconds
    // operator""s()
    // { return __check_overflow<chrono::seconds, _Digits...>(); }
    // 使用 operator"" 关键字来定义一个自定义的字面量运算符，其后跟着 s，表示该字面量的后缀是 s，返回的是duration类型的值。
    cout << "overload operator\"\"s: " << max_time << endl;


    std::future<void> f_async = std::async(std::launch::async, []() {
        std::this_thread::sleep_for(1s); // 等待一秒
    });
    std::future<void> f_deferred = std::async(std::launch::deferred, []() {});
    // wait_for()用来等待异步任务的完成，可以观看1ms和1000ms的区别
    // cout << (f_async.wait_for(1000ms) == std::future_status::ready) << endl;
    cout << (f_async.wait_for(100ms) == std::future_status::ready) << endl;
    cout << (f_deferred.wait_for(1ns) == std::future_status::ready) << endl;

    return 0;
}