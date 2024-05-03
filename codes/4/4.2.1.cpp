//
// Created by Samsara on 2024/5/2.
// File name: 4.cpp
// Description: 通过观察launch::deferred和launch::async的运行时间，得出二者运行方法差异
//
#include <iostream>
#include <future>
#include <thread>
#include <chrono>
#include <unistd.h>

using namespace std;

int getValueInFuture(int x)
{
    sleep(1);
    return x;
}

int main()
{
    auto start = chrono::steady_clock::now();
    // 异步执行任务，并且延迟执行
    future<int> result = async(launch::deferred, getValueInFuture, 2);
    sleep(1);
    // 使用get()时才产生线程
    cout << "Deferred result: " << result.get() << endl;
    auto end = chrono::steady_clock::now();
    cout << "Time taken for deferred task: " << chrono::duration<double>(end - start).count() << " seconds" << endl;

    start = chrono::steady_clock::now();
    // 异步执行任务，并且立即执行
    // 立马产生线程
    future<int> result1 = async(launch::async, getValueInFuture, 2);
    sleep(1);
    cout << "Async result: " << result1.get() << endl;
    end = chrono::steady_clock::now();
    cout << "Time taken for async task: " << chrono::duration<double>(end - start).count() << " seconds" << endl;

    return 0;
}
