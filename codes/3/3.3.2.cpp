//
// Created by Samsara on 2024/4/28.
// File name: 3.cpp
// Description: 观察write和read的输出，即可知道结果
//
#include <iostream>
#include <thread>
#include <mutex>
#include <shared_mutex>
#include <vector>

using namespace std;

shared_mutex m;

void write(int id)
{
    lock_guard<shared_mutex> lock(m);
    // 只有一个线程可以执行这里的代码
    cout << "write with id: ";

    for (int i = 0; i < 10; ++i)
        cout << id;
    cout << endl;
}

void read(int id)
{
    shared_lock<shared_mutex> lock(m);
    // 多个线程可以同时执行这里的代码
    cout << "read with id: " << id << endl;

    for (int i = 0; i < 10; ++i)
        cout << id;
    cout << endl;
}

int main()
{
    int n = 10;
    vector<thread> v;
    for (int i = 0; i < n; ++i)
    {
        v.emplace_back(write, i);
    }
    for (auto &t : v)
        t.join();
    v.clear();
    for (int i = 0; i < n; ++i)
    {
        v.emplace_back(read, i);
    }
    for (auto &t : v)
        t.join();
    return 0;
}