//
// Created by Samsara on 2024/5/23.
// File name: 6.cpp
// Description: 
//
#include <iostream>
#include <atomic>
#include <shared_mutex>
#include <queue>
#include <condition_variable>

using namespace std;

template<typename T>
class threadsafe_queue
{
private:
    mutable std::mutex mut;
    std::queue<T> data_queue;
    std::condition_variable data_cond;

public:
    threadsafe_queue() {}

    void push(T data)
    {
        std::lock_guard<std::mutex> lk(mut);
        data_queue.push(std::move(data));
        data_cond.notify_one();  // 1
    }

    // 为什么没有返回值类型为T的pop函数？因为它是不安全的
    // 在异常安全的面向对象设计中，一种重要的理念是"Strong Exception Safety"，即函数要么成功完成，要么所有状态回滚且没有任何副作用。
    // 比如下面的T res = data_queue.front() -> return res;
    // 如果res很大或者T的拷贝构造函数引发异常，那么可能会导致return res时抛出了异常内存分配失败，从而抛出构造的异常
    // 但此时data_queue.pop()已经执行，导致数据丢失
    T pop()
    {
        std::lock_guard<std::mutex> lk(mut);
        if (data_queue.empty())
        {
            throw std::out_of_range("pop from empty queue");
        }
        T res = data_queue.front();
        data_queue.pop();
        return res;
    }

    // 为了解决上面的问题，我们可以使用引用传递的方式
    // 如果T的拷贝构造函数抛出异常，在wait_and_pop(T& value)内部，原始队列的状态并未改变，因此是异常安全的。
    // 只有当我们能够成功获取元素后，我们才从队列中移除元素，这样可以保证原子性和异常安全性。
    void wait_and_pop(T &value)  // 2
    {
        std::unique_lock<std::mutex> lk(mut);
        data_cond.wait(lk, [this] { return !data_queue.empty(); });
        value = std::move(data_queue.front());
        data_queue.pop();
    }

    // 当std::shared_ptr被返回时，C++11引入了一项称为"返回值优化"（RVO）或"命名返回值优化"（NRVO）的技术。
    // 这个技术可以避免在返回本地对象时进行的额外的复制或移动操作。并且，即使在没有RVO的情况下，
    // 从函数返回的std::shared_ptr对象也将通过移动语义（而非复制语义）进行传递，这个过程相对低成本且不会抛出异常。
    std::shared_ptr<T> wait_and_pop()  // 3
    {
        std::unique_lock<std::mutex> lk(mut);
        data_cond.wait(lk, [this] { return !data_queue.empty(); });  // 4
        std::shared_ptr<T> res(
                std::make_shared<T>(std::move(data_queue.front())));
        data_queue.pop();
        return res;
    }

    bool try_pop(T &value)
    {
        std::lock_guard<std::mutex> lk(mut);
        if (data_queue.empty())
            return false;
        value = std::move(data_queue.front());
        data_queue.pop();
        return true;
    }

    std::shared_ptr<T> try_pop()
    {
        std::lock_guard<std::mutex> lk(mut);
        if (data_queue.empty())
            return std::shared_ptr<T>();  // 5
        std::shared_ptr<T> res(
                std::make_shared<T>(std::move(data_queue.front())));
        data_queue.pop();
        return res;
    }

    bool empty() const
    {
        std::lock_guard<std::mutex> lk(mut);
        return data_queue.empty();
    }
};