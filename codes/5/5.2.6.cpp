//
// Created by Samsara on 2024/5/14.
// File name: 5.cpp
// Description:
// 在 C++ 中，如果我们想要使用 std::atomic 来创建一个原子变量，我们需要确保该类型是“trivially copyable”的，
// 即可以使用标准的拷贝赋值运算符（=）进行拷贝。这是因为 std::atomic 为了实现原子操作，
// 常会使用底层的机器指令或者特殊的 CPU 指令，而这些指令通常对于基本的内存布局和操作有一些要求。
// 使用拷贝赋值运算符可以确保原子变量的拷贝操作是快速且有效的。如果类型没有拷贝赋值运算符，
// 那么原子变量的拷贝可能会导致更复杂的操作，比如调用用户自定义的拷贝构造函数和析构函数，
// 这可能会增加原子操作的开销，甚至导致一些未定义行为。因此，为了确保原子操作的正确性和性能，
// std::atomic 要求所操作的类型必须是“trivially copyable”的，也就是必须支持拷贝赋值运算符。
//
#include <iostream>
#include <atomic>

using namespace std;


struct node {
    int a;
    int b;
    // node& operator=(const node& n) = delete;
};

int main()
{
    atomic<node> a({1, 2});
    a.store({3, 4});
    cout << "store for struct: " << endl;
    cout << "after: " << a.load().a << " " << a.load().b << endl; // "3 4"
    return 0;
}