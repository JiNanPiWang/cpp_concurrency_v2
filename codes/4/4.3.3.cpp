//
// Created by Samsara on 2024/5/7.
// File name: 4.cpp
// Description: 测试condition_variable的wait_until函数，等待超时
//
#include <iostream>
#include <condition_variable>
#include <chrono>
#include <mutex>

using namespace std;

bool ready = false;
condition_variable cv;
mutex mtx;

void worker()
{
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    std::unique_lock<std::mutex> lock(mtx);
    ready = true;
    cv.notify_one();
}

int main()
{
    std::thread t1(worker);

    // 改变时间为1ms看看？
    auto _timeout = std::chrono::steady_clock::now() + std::chrono::milliseconds(1000);
    std::unique_lock<std::mutex> lock(mtx);

    // wait_until返回值是bool，这里等待ready是true，如果超时就退出
    if (cv.wait_until(lock, _timeout, [](){return ready;}))
    {
        cout << "not timeout" << endl;
    }
    else
    {
        cout << "timeout" << endl;
    }
    t1.join();

    return 0;
}