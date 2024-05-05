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

    std::future<int> fu = std::async(std::launch::async, [](){return 1;});
    std::shared_future<int> sfu = fu.share();
    // 当您第一次从 std::future 对象调用 .share() 方法时，
    // 该 std::future 对象会转移其状态给一个新创建的 std::shared_future 对象 (sfu)，
    // 并且原 std::future 对象 (fu) 会变为无效状态。
    std::shared_future<int> sfu1 = sfu;
    std::shared_future<int> sfu2 = sfu;
    std::shared_future<int> sfu3 = sfu;

    cout << "shared_future: " << sfu.get() << endl;
    cout << "shared_future1: " << sfu1.get() << endl;
    cout << "shared_future2: " << sfu2.get() << endl;
    cout << "shared_future3: " << sfu3.get() << endl;

    return 0;
}
