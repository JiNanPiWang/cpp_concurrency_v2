//
// Created by Samsara on 2024/5/29.
// File name: 6.cpp
// Description: tail指向的是队列最后一个节点的下一个节点（虚）
//
#include <iostream>
#include <memory>
#include <cassert>
#include <thread>
#include <vector>
#include <mutex>

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

    node *get_tail()
    {
        lock_guard<mutex> lk(tail_mutex);
        return tail;
    }

    unique_ptr<node> pop_head()
    {
        lock_guard<mutex> lk(head_mutex);
        if (head.get() == get_tail())
        {
            return nullptr;
        }
        auto old_head = std::move(head); // 能够提供更好的保护，因为它保留了旧的头节点，可以在异常处理代码中恢复状态。
        head = std::move(old_head->next);
        return old_head;
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

    std::shared_ptr<T> try_pop()
    {
        auto old_head = pop_head();
        if (old_head == nullptr)
            return shared_ptr<T>();
        else
            return old_head->data;
    }

    void push(T new_value)
    {
        // p指向的是队列最后一个节点的下一个节点（虚）
        auto new_data = make_shared<T>(std::move(new_value));
        std::unique_ptr<node> p(new node);
        auto new_tail = p.get();
        lock_guard<mutex> lk(tail_mutex);
        tail->data = new_data;
        tail->next = std::move(p);
        tail = new_tail;
    }
};

int main()
{
    threadsafe_queue<int> q;

    // Function to push elements into the queue in a thread-safe manner
    auto push_function = [&q](int start, int end) {
        for (int i = start; i <= end; ++i) {
            q.push(i);
            cout << "Pushed: " << i << endl;
        }
    };

    // Create a vector to hold threads
    vector<thread> threads;

    // Define the number of threads
    const int num_threads = 5;

    // Define the range of numbers to be pushed by each thread
    const int per_thread_count = 20;

    // Start threads
    for (int i = 0; i < num_threads; ++i) {
        threads.emplace_back(push_function, i * per_thread_count + 1, (i + 1) * per_thread_count);
    }

    // Join threads
    for (auto &t : threads) {
        t.join();
    }

    // Check if the queue is empty
    cout << "Queue empty: " << (q.empty() ? "Yes" : "No") << endl;

    return 0;
}
