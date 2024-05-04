//
// Created by Samsara on 2024/5/3.
// File name: 4.cpp
// Description:
//
#include <iostream>
#include <thread>
#include <functional>
#include <future>

using namespace std;

int main()
{
    // 将一个可调用对象包装成一个异步任务，您可以将这个任务传递给一个线程执行。
    // std::async 函数是一个高级工具，用于启动一个异步任务，并返回一个 std::future 对象，以便稍后获取任务的结果。
    packaged_task<int(int)> task([](int x){ return x * x; });
    future<int> fu = task.get_future();
    thread t(std::move(task), 2);

    cout << fu.get() << endl;
    t.join();

    return 0;
}
