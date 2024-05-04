//
// Created by Samsara on 2024/5/4.
// File name: 4.cpp
// Description: 如果尝试计算负数的平方根，f 函数会抛出一个 std::runtime_error 异常。
// 这个异常会被捕获，并通过 promise.set_exception 方法设置到 promise 对象中。
// 在 main 函数中，当调用 future.get() 时，如果存在异常，它会被重新抛出，然后通过 catch 块捕获并处理。
//
#include <iostream>
#include <future>
#include <cmath>

using namespace std;

void f(std::promise<double> &p, const int &val)
{
    try
    {
        if (val < 0)
        {
            throw std::runtime_error("Negative value"); // 抛出异常
        }
        p.set_value(sqrt(val));
    }
    catch (...)
    {
        p.set_exception(std::current_exception()); // 捕获并设置异常
    }
}

int main()
{
    std::promise<double> prom;
    std::future<double> fu = prom.get_future();
    thread t(f, std::ref(prom), -1);
    try
    {
        auto x = fu.get(); // 尝试获取结果，可能会抛出异常
        cout << "future: " << x << endl;
    }
    catch (const std::exception &e)
    {
        cout << "Caught exception: " << e.what() << endl; // 处理异常
    }
    t.join();
    return 0;
}
