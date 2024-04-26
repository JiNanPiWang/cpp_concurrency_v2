//
// Created by Samsara on 2024/4/25.
// File name: 3.cpp
// Description: 
//
#include <iostream>
#include <stack>
#include <mutex>
#include <thread>
#include <vector>
#include <algorithm>

using namespace std;

template<typename T>
class safe_stack
{
private:
    stack<T> sk;
    mutex m;

public:
    unique_ptr<T> pop()
    {
        scoped_lock<mutex> lock(m);
        if (sk.empty()) {
            return nullptr; // 需要处理空栈的情况
        }
        auto ptr = make_unique<T>(sk.top());
        sk.pop();
        return ptr;
    }

    T top()
    {
        return sk.top();
    }

    void push(int x)
    {
        scoped_lock<mutex> lock(m);
        sk.push(x);
    }
};

int main()
{
    int n = 100;
    safe_stack<int> sk;
    vector<thread> v;
    for (int i = 0; i < n; ++i)
    {
        v.emplace_back(&safe_stack<int>::push, &sk, i);
    }
    for (auto &th : v)
        th.join();
    v.clear();

    vector<int> result;
    for (int i = 0; i < n; ++i)
    {
        v.emplace_back([&sk, &result](){
            auto p = sk.pop();
            if (p != nullptr)
                result.push_back(*p);
            else
                cout << "NULL!" << endl;
        });
    }
    for (auto &th : v)
        th.join();

    // 在所有线程结束后，对结果进行排序
    std::sort(result.begin(), result.end());

    // 验证排序后的结果是否为0~n-1
    for (int i = 0; i < n; ++i) {
        if (result[i] != i) {
            std::cout << "The sorted result is not 0~n-1" << std::endl;
            return 1;
        }
    }
    std::cout << "The sorted result is 0~n-1" << std::endl;

    return 0;
}