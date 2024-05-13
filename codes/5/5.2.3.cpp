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

    return 0;
}