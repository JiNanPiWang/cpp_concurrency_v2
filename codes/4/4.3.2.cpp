//
// Created by Samsara on 2024/5/6.
// File name: 4.cpp
// Description: 
//
#include <iostream>
#include <chrono>
#include <unistd.h>

using namespace std;

int main()
{
    auto start_time = std::chrono::system_clock::now();
    auto end_time = start_time + std::chrono::seconds(90);
    // 60s算一次，也就是计算多少分钟了
    auto duration = std::chrono::duration_cast<std::chrono::duration<int, std::ratio<60, 1>>>(end_time - start_time);
    cout << duration << endl;
    return 0;
}