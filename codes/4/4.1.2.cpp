//
// Created by Samsara on 2024/4/30.
// File name: 4.cpp
// Description:
//
#include <iostream>
#include <queue>
#include <mutex>
#include <condition_variable>

using namespace std;

template <typename T>
class safe_queue
{
private:
    queue<T> q;
    mutex mtx;
    condition_variable cv;

public:
    void push(T x)
    {
        {
            lock_guard<mutex> lock(mtx);
            q.push(x);
            cout << "push data " << x << endl;
        }
        cv.notify_one();
    }

    void wait_and_pop()
    {
        unique_lock<mutex> lock(mtx);
        cv.wait(lock, [this]{return !q.empty();});
        auto tmp = q.front();
        q.pop();
        cout << "pop data " << tmp << endl;
    }
};

int main()
{
    safe_queue<int> sq;
    thread t1(&safe_queue<int>::wait_and_pop, &sq);
    thread t2(&safe_queue<int>::push, &sq, 10);
    t1.join();
    t2.join();

    return 0;
}