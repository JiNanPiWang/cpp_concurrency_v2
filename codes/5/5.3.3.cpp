//
// Created by Samsara on 2024/5/16.
// File name: 5.cpp
// Description: std::memory_order_consume更关注于数据依赖关系，而不是控制依赖关系。详见下面的代码：
//
#include <iostream>
#include <atomic>
#include <thread>
#include <cassert>
#include <chrono>

using namespace std;

struct X
{
    int i;
    std::string s;
};

std::atomic<X *> p;
std::atomic<int> a;

void create_x()
{
    X *x = new X;
    x->i = 42;
    x->s = "hello";
    std::this_thread::sleep_for(std::chrono::seconds(1));

    a.store(99, std::memory_order_relaxed);  // 1
    p.store(x, std::memory_order_release);  // 2
}

void use_x()
{
    X *x;
    while (!(x = p.load(std::memory_order_consume)))  // 3 这里使用了std::memory_order_consume
        std::this_thread::sleep_for(std::chrono::microseconds(1));

    // 因为加载 p 变量的操作采用了 std::memory_order_consume 内存序，这表明后续对 X 对象的访问是依赖于加载 p 变量的。
    // 所以4和5的断言是安全的
    assert(x->i == 42);  // 4
    assert(x->s == "hello");  // 5
    // 但是6的断言是不安全的，因为a与x之间没有数据依赖关系，且a的存储操作采用了 std::memory_order_relaxed 内存序
    // 所以6的断言可能会失败
    assert(a.load(std::memory_order_relaxed) == 99);  // 6
}

int main()
{
    std::thread t1(create_x);
    std::thread t2(use_x);
    t1.join();
    t2.join();
}