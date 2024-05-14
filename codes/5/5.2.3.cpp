//
// Created by Samsara on 2024/5/13.
// File name: 5.cpp
// Description: 
//
#include <iostream>
#include <atomic>
#include <thread>

using namespace std;

std::atomic<bool> x, y;
std::atomic<int> z;

void write_x_then_y()
{
    // 保证读入y之前，一定读入了x，read_y_then_x()函数里面的z不会是0
    x.store(true, std::memory_order_relaxed);
    y.store(true, std::memory_order_release);
}

void read_y_then_x() {
    while (!y.load(std::memory_order_acquire));
    if (x.load(std::memory_order_relaxed))
    {
        ++z;
    }
}

int main()
{
    x = false;
    y = false;

    std::thread a(write_x_then_y);
    std::thread b(read_y_then_x);

    a.join();
    b.join();

    cout << z.load() << endl;

    std::atomic<bool> c, d;
    bool expected = false;
    // compare_exchange_weak()和compare_exchange_strong()的区别在于，compare_exchange_weak()可能会失败，而compare_exchange_strong()不会
    // compare_exchange_weak()和compare_exchange_strong()的第一个参数是期望值，第二个参数是要设置的值，
    // 第三个参数是成功时的内存序，第四个参数是失败时的内存序
    // 比如下面的代码，c和d的值都是false，所以compare_exchange_weak()和compare_exchange_strong()都会将c和d的值设置为true
    // 第一个内存徐代表成功时前面的操作都要完成
    c.compare_exchange_weak(expected, true, std::memory_order_release, std::memory_order_relaxed);
    d.compare_exchange_strong(expected, true, std::memory_order_release, std::memory_order_relaxed);

    cout << c.load() << endl;
    cout << d.load() << endl;

    auto exchange = c.exchange(false, std::memory_order_relaxed);
    // exchange()函数会返回c之前的值，所以exchange的值是1
    cout << "exchange: " << exchange << endl;
    cout << "After exchange: " << c.load() << endl;

    return 0;
}