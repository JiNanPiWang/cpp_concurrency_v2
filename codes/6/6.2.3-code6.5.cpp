//
// Created by Samsara on 2024/5/28.
// File name: 6.cpp
// Description: tail指向的是队列最后一个节点的下一个节点（虚）
//
#include <iostream>
#include <memory>
#include <cassert>

using namespace std;

template<typename T>
class queue
{
private:
    struct node
    {
        shared_ptr<T> data;
        std::unique_ptr<node> next;
    };

    std::unique_ptr<node> head;  // 1
    node *tail = nullptr;  // 2

public:
    queue() : head(new node), tail(head.get()) {}

    queue(const queue &other) = delete;

    queue &operator=(const queue &other) = delete;

    bool empty()
    {
        return tail == head.get();
    }

    std::shared_ptr<T> try_pop()
    {
        if (empty())
        {
            return std::shared_ptr<T>();
        }
        auto res = head->data;
        auto old_head = std::move(head); // 能够提供更好的保护，因为它保留了旧的头节点，可以在异常处理代码中恢复状态。
        head = std::move(old_head->next);
        return res;
    }

    void push(T new_value)
    {
        // p指向的是队列最后一个节点的下一个节点（虚）
        tail->data = make_shared<T>(std::move(new_value));
        std::unique_ptr<node> p(new node);
        auto new_tail = p.get();
        tail->next = std::move(p);
        tail = new_tail;
    }
};

int main()
{
    queue<int> q;

    // Test 1: Check if the queue is initially empty
    assert(q.empty());

    // Test 2: Push an element and check if it can be popped
    q.push(1);
    auto res = q.try_pop();
    assert(res && *res == 1);

    // Test 3: Check if the queue is empty after popping the only element
    assert(q.empty());

    // Test 4: Try to pop from an empty queue and check if it returns a null shared_ptr
    res = q.try_pop();
    assert(!res);

    // Test 5: Push multiple elements and check if they can be popped in the correct order
    for (int i = 0; i < 10; ++i)
    {
        q.push(i);
    }
    assert(!q.empty());
    for (int i = 0; i < 10; ++i)
    {
        res = q.try_pop();
        assert(res && *res == i);
    }

    // Test 6: Check if the queue is empty after popping all elements
    assert(q.empty());

    return 0;
}