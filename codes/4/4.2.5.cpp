//
// Created by Samsara on 2024/5/5.
// File name: 4.cpp
// Description: 
//
#include <iostream>
#include <future>
#include <cassert>

using namespace std;

int main()
{
    std::promise<int> p;
    std::future<int> f(p.get_future());
    assert(f.valid());  // 1 期望值 f 是合法的
    std::shared_future<int> sf(std::move(f));
    assert(!f.valid());  // 2 期望值 f 现在是不合法的
    assert(sf.valid());  // 3 sf 现在是合法的
    return 0;
}
