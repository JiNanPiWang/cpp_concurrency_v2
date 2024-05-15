//
// Created by Samsara on 2024/5/14.
// File name: 5.cpp
// Description: atomic与shared_ptr
// shared_ptr一个实际的例子是一个简单的图形用户界面（GUI）程序，
// 其中多个控件（如按钮、文本框、标签等）可能需要共享对同一个事件处理器的访问权限。
//
#include <atomic>
#include <memory>
#include <thread>
#include <iostream>

std::shared_ptr<int> ptr;

void update_ptr()
{
    std::atomic_store_explicit(&ptr, std::make_shared<int>(1), std::memory_order_acquire);
}

int main()
{
    ptr = std::make_shared<int>(0);
    std::cout << "old ptr value: " << *ptr << std::endl; // "0"
    std::atomic_store_explicit(&ptr, std::make_shared<int>(1), std::memory_order_acquire);
    std::cout << "new ptr value: " << *ptr << std::endl; // "1
    return 0;
}