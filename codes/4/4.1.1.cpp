//
// Created by Samsara on 2024/4/29.
// File name: 4.cpp
// Description:
//
#include <iostream>
#include <thread>
#include <condition_variable>
#include <mutex>
#include <queue>
#include <chrono>

using namespace std;

queue<int> q;
condition_variable cv;
mutex mtx;

void write(int x)
{
    for (int i = 1; i <= x; ++i)
    {
        {
            lock_guard<mutex> lock(mtx);
            cout << "write data " << i << endl;
            q.push(i);
        }
        cv.notify_one();

        // 等待和不等待，区别
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}

void read(int x)
{
    int data;
    do
    {
        unique_lock<mutex> lock(mtx);
        // 当 cv.notify_one(); 被调用时，它将唤醒一个等待的线程，
        // 但这个线程在继续执行之前，必须重新获取之前释放的互斥锁（mtx）。
        // 如果 write 线程仍然持有锁或者在等待线程尝试获取锁之前再次获取了锁，那么 read 纑程将不会立即执行。
        cv.wait(lock, [](){return !q.empty();});
        data = q.front();
        q.pop();
        cout << "read data " << data << endl;
    } while (data != x);
}

int main()
{
    int n = 10000;
    thread t1(write, n);
    thread t2(read, n);
    t1.join();
    t2.join();
    return 0;
}