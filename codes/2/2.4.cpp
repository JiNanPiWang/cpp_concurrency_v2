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

const int N = 1e8 + 10;
const auto num_threads = thread::hardware_concurrency();

long long accu(vector<int> v)
{
    auto start = std::chrono::high_resolution_clock::now();

    long long result = accumulate(v.begin(), v.end(), 0);

    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = duration_cast<std::chrono::milliseconds>(stop - start);

    cout << "Accumulate time: " << duration.count() << "ms" << endl;

    return result;
}

int main()
{
    vector<int> v;
    for (int i = 0; i < N; ++i)
        v.emplace_back(rand());

    auto r1 = accu(v);

    return 0;
}