//
// Created by Samsara on 2024/4/24.
// File name: 2.cpp
// Description: 
//
#include <iostream>
#include <thread>
#include <iterator>
#include <vector>
#include <numeric>
#include <chrono>

using namespace std;

const int N = 1e8;
const auto num_threads = 8;

long long accu(const vector<int> &v)
{
    auto start = std::chrono::high_resolution_clock::now();

    long long result = accumulate(v.begin(), v.end(), 0ll);

    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = duration_cast<std::chrono::milliseconds>(stop - start);

    cout << "Accumulate time: " << duration.count() << "ms" << endl;

    return result;
}

long long concurrent_accu(const vector<int> &v)
{
    vector<long long> result(num_threads, 0);
    vector<thread> threads;
    auto interval = distance(v.begin(), v.end()) / num_threads;

    auto start = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < num_threads; ++i)
    {
        threads.emplace_back(
                // result 应该是以引用方式捕获，如果不以引用方式捕获，每个线程将会得到 result 的一个拷贝，而不是操作同一个共享的容器。
                // 全局变量在程序的任何部分都是可见的
                [&result, interval, &v](int id)
                {
                    auto st = v.begin() + interval * id;
                    auto ed = v.begin() + interval * (id + 1);
                    if (id == num_threads - 1)
                        ed = v.end();
                    result[id] = accumulate(st, ed, 0ll);
//                    result[id] = id;
                }, i
        );
    }
    for (auto &th : threads)
        th.join();

    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = duration_cast<std::chrono::milliseconds>(stop - start);

    cout << "Concurrent accumulate time: " << duration.count() << "ms" << endl;

    return std::accumulate(result.begin(), result.end(), 0ll);
}

int main()
{
    vector<int> v;
    for (int i = 0; i < N; ++i)
        v.emplace_back(rand());

    auto r1 = accu(v);
    auto r2 = concurrent_accu(v);
    cout << "r1: " << r1 << endl;
    cout << "r2: " << r2 << endl;

    if (r1 == r2)
        cout << "Correct!" << endl;

    return 0;
}