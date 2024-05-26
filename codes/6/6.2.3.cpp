//
// Created by Samsara on 2024/5/23.
// File name: 6.cpp
// Description: 自己实现的，和书上的不一样
//
#include <iostream>
#include <memory>
#include <cassert>

using namespace std;

template<typename T>
class queue
{
private:
    struct Node
    {
        shared_ptr<Node> next;
        T val;

        explicit Node(T _val) : val(std::move(_val)) {}
    };

    shared_ptr<Node> head, tail;
public:
    queue() = default;

    queue(const queue &q) = delete;

    queue &operator=(const queue &q) = delete;

    bool empty()
    {
        return (this->head == nullptr && this->tail == nullptr);
    }

    void push(T val)
    {
        // std::make_shared函数会返回一个std::shared_ptr类型的智能指针
        auto new_node = make_shared<Node>(std::move(val));
        if (this->empty())
        {
            this->head = new_node;
            this->tail = new_node;
        }
        else
        {
            // 队尾的下一位是新节点
            this->tail->next = new_node;
            this->tail = new_node;
        }
    }

    shared_ptr<T> try_pop()
    {
        if (this->empty())
            return shared_ptr<T>();
        auto return_val = make_shared<T>(this->head->val);
        if (this->head == this->tail)
            this->tail = this->tail->next;
        this->head = this->head->next;
        return return_val;
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