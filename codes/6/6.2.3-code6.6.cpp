//
// Created by Samsara on 2024/5/29.
// File name: 6.cpp
// Description: tail指向的是队列最后一个节点的下一个节点（虚）
//
#include <iostream>
#include <memory>
#include <cassert>
#include <thread>
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
    threadsafe_queue<int> queue;

    // 测试push函数
    cout << "Pushing elements into the queue." << endl;
    for(int i = 0; i < 10; ++i)
    {
        queue.push(i);
    }

    // 测试empty函数
    cout << "Is queue empty? " << (queue.empty() ? "Yes" : "No") << endl;

    // 测试try_pop函数
    cout << "Popping elements from the queue:" << endl;
    for(int i = 0; i < 10; ++i)
    {
        shared_ptr<int> value = queue.try_pop();
        if(value != nullptr)
        {
            cout << *value << ",";
        }
    }

    // 检查队列是否为空
    cout << "\nIs queue empty? " << (queue.empty() ? "Yes" : "No") << endl;

    return 0;
}