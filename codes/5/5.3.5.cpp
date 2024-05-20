//
// Created by Samsara on 2024/5/20.
// File name: 5.cpp
// Description: atomic_thread_fence
// 屏障，会决定前后的内存操作按照指定的顺序进行
// release：该屏障之前的所有读写操作都在该屏障之后的写操作之前完成
// acquire：该屏障之后的所有读写操作都在该屏障之前的读操作之后完成
//
#include <atomic>
#include <thread>
#include <assert.h>

std::atomic<bool> x, y;
std::atomic<int> z;

void write_x_then_y()
{
    x.store(true,std::memory_order_relaxed);  // 1
    std::atomic_thread_fence(std::memory_order_release);  // 2   前面的1必须在2之前完成
    y.store(true,std::memory_order_relaxed);  // 3
}

void read_y_then_x()
{
    while(!y.load(std::memory_order_relaxed));  // 4
    std::atomic_thread_fence(std::memory_order_acquire);  // 5     6必须在5之后
    if(x.load(std::memory_order_relaxed))  // 6
        ++z;
}

int main()
{
    x = false;
    y = false;
    z = 0;
    std::thread a(write_x_then_y);
    std::thread b(read_y_then_x);
    a.join();
    b.join();
    assert(z.load() != 0);  // 7
}