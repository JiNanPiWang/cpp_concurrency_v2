//
// Created by Samsara on 2024/6/8.
// File name: 6.cpp
// Description: try and pop
//
#include <iostream>
#include <memory>
#include <condition_variable>
#include <thread>
#include <vector>
#include <mutex>
#include <future>

using namespace std;

template<typename T>
class threadsafe_queue
{
private:
    struct node
    {
        shared_ptr<T> data;
        std::unique_ptr<node> next;
    };

    std::unique_ptr<node> head;
    node *tail = nullptr;
    std::mutex head_mutex, tail_mutex;
    condition_variable data_cond;

    node *get_tail()
    {
        lock_guard<mutex> lk(tail_mutex);
        return tail;
    }

    // for wait_for_data, no detect for empty queue
    unique_ptr<node> pop_head()
    {
        auto old_head = std::move(head); // 能够提供更好的保护，因为它保留了旧的头节点，可以在异常处理代码中恢复状态。
        head = std::move(old_head->next);
        return old_head;
    }

    unique_ptr<node> try_pop_head()
    {
        lock_guard<mutex> head_lock(head_mutex);
        if (head.get() == get_tail())
            return unique_ptr<node>();
        return pop_head();
    }

    unique_ptr<node> try_pop_head(T &value)
    {
        lock_guard<mutex> head_lock(head_mutex);
        if (head.get() == get_tail())
            return nullptr;
        value = move(*head->data);
        return pop_head();
    }

    unique_lock<mutex> wait_for_data()
    {
        unique_lock<mutex> head_lock(head_mutex);
        // 不需要上锁，cv被通知后会先重新获得锁，再检查条件是否满足
        data_cond.wait(head_lock, [&]() { return head.get() != get_tail(); });
        return std::move(head_lock);
    }

    unique_ptr<node> wait_pop_head()
    {
        auto head_lock = wait_for_data();
        return pop_head();
    }

    unique_ptr<node> wait_pop_head(T &value) // unique_ptr类型，为了资源的正确管理和释放以及简洁
    {
        auto head_lock = wait_for_data();
        value = *head->data;
        return pop_head();
    }

public:
    threadsafe_queue() : head(new node), tail(head.get()) {}

    threadsafe_queue(const threadsafe_queue &other) = delete;

    threadsafe_queue &operator=(const threadsafe_queue &other) = delete;

    bool empty() // 仅供main测试，这不符合细粒度锁的理念
    {
        scoped_lock lk(head_mutex, tail_mutex);
        return tail == head.get();
    }

    void push(T new_value)
    {
        // p指向的是队列最后一个节点的下一个节点（虚）
        auto new_data = make_shared<T>(std::move(new_value));
        std::unique_ptr<node> p(new node);
        auto new_tail = p.get();
        {
            lock_guard<mutex> lk(tail_mutex);
            tail->data = new_data;
            tail->next = std::move(p);
            tail = new_tail;
        }
        data_cond.notify_one();
    }

    shared_ptr<T> try_pop()
    {
        auto old_head = try_pop_head();
        if (old_head == nullptr)
            return nullptr;
        else
            return old_head->data; // nullptr？不用，因为空的时候会wait
    }

    bool try_pop(T &value)
    {
        auto old_head = try_pop_head(value);
        if (old_head == nullptr)
            return false;
        else
            return true; // nullptr？不用，因为空的时候会wait
    }

    shared_ptr<T> wait_and_pop()
    {
        auto old_head = wait_pop_head();
        return old_head->data; // nullptr？不用，因为空的时候会wait
    }

    void wait_and_pop(T &value)
    {
        auto old_head = wait_pop_head(value);
    }

};

int main()
{
    threadsafe_queue<int> tsq;

    // 测试 push 和 wait_and_pop
    tsq.push(1);
    tsq.push(2);
    tsq.push(3);

    auto item = tsq.wait_and_pop();
    if (item)
        cout << "Popped: " << *item << endl;
    else
        cout << "Queue is empty!" << endl;

    item = tsq.wait_and_pop();
    if (item)
        cout << "Popped: " << *item << endl;
    else
        cout << "Queue is empty!" << endl;

    item = tsq.wait_and_pop();
    if (item)
        cout << "Popped: " << *item << endl;
    else
        cout << "Queue is empty!" << endl;

    item = tsq.try_pop();
    if (item)
        cout << "Popped: " << item << endl;
    else
        cout << "Queue is empty!" << endl;

    // 测试 wait_and_pop
    tsq.push(4);
    tsq.push(5);

    auto pop_thread = thread([&tsq]()
                             {
                                 this_thread::sleep_for(chrono::milliseconds(500)); // 等待2秒以模拟工作
                                 auto item = tsq.wait_and_pop();
                                 cout << "Wait and popped: " << *item << endl;
                             });

    tsq.push(6);

    auto item2 = tsq.wait_and_pop();
    if (item2)
        cout << "Wait and popped: " << *item2 << endl;

    pop_thread.join();

    // 测试并发 push 和 pop
    vector<thread> threads;

    // 生产者线程
    for (int i = 0; i < 10; ++i)
    {
        threads.emplace_back([&tsq, i]()
                             {
                                 tsq.push(i);
                                 cout << "Pushed: " << i << endl;
                             });
    }

    // 消费者线程
    for (int i = 0; i < 10; ++i)
    {
        threads.emplace_back([&tsq]()
                             {
                                 auto item = tsq.wait_and_pop();
                                 cout << "Wait and popped in thread: " << *item << endl;
                             });
    }

    for (auto &t: threads)
    {
        t.join();
    }

    return 0;
}
