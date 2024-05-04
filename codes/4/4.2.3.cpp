//
// Created by Samsara on 2024/5/4.
// File name: 4.cpp
// Description:
// 需要执行函数的时候，可以用packaged_task
// 需要手动设置值的时候，可以用promise
//
#include <iostream>
#include <future>

using namespace std;

void f(std::promise<long long> &p)
{
    p.set_value(100);
}

int main()
{
    std::promise<long long> prom;
    std::future<long long> fu = prom.get_future();
    thread t(f, std::ref(prom));
    auto x = fu.get();
    cout << "future: " << x << endl;
    t.join();
    return 0;
}
