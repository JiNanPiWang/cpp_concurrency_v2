//
// Created by Samsara on 2024/4/28.
// File name: 3.cpp
// Description: 
//
#include <iostream>
#include <thread>
#include <mutex>

std::once_flag flag;

void do_once(int id) {
    std::call_once(flag, [id]() {
        std::cout << "Called by id: " << id << std::endl;
    });
}

int main() {
    std::thread t1(do_once, std::thread::hardware_concurrency());
    std::thread t2(do_once, std::thread::hardware_concurrency());
    std::thread t3(do_once, std::thread::hardware_concurrency());

    t1.join();
    t2.join();
    t3.join();

    return 0;
}