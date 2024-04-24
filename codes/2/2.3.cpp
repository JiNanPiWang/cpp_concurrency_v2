//
// Created by Samsara on 2024/4/24.
// File name: 2.cpp
// Description: 
//
#include <iostream>
#include <thread>

using namespace std;

void some_function() { return; }

void some_other_function() { return; }

int main()
{
    std::thread t1(some_function);            // 1
    std::thread t2 = std::move(t1);            // 2
    t1 = std::thread(some_other_function);    // 3
    std::thread t3;                            // 4
    t3 = std::move(t2);                        // 5
    // t1原来的线程被强制终结
    // 当一个线程被std::terminate调用时，整个程序将会被终止。
    t1 = std::move(t3);                        // 6 赋值操作将使程序崩溃
    return 0;
}