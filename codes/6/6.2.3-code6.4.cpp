//
// Created by Samsara on 2024/5/23.
// File name: 6.cpp
// Description: 
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
        T data;
        std::unique_ptr<node> next;

        explicit node(T data_) : data(std::move(data_)) {}
    };

    std::unique_ptr<node> head;  // 1
    node* tail = nullptr;  // 2

public:
    queue() = default;
    queue(const queue& other)=delete;
    queue& operator=(const queue& other)=delete;
    std::shared_ptr<T> try_pop()
    {
        if(head == nullptr)
        {
            return std::shared_ptr<T>();
        }
        std::shared_ptr<T> const res(std::make_shared<T>(std::move(head->data)));
        std::unique_ptr<node> const old_head=std::move(head);
        head=std::move(old_head->next);  // 3
        return res;
    }

    void push(T new_value)
    {
        // 新建节点
        std::unique_ptr<node> p(new node(std::move(new_value)));
        node* const new_tail=p.get();
        if(tail != nullptr) // 如果队列已经有元素
        {
            tail->next=std::move(p);  // 4
        }
        else // 如果队列没元素
        {
            head=std::move(p);  // 5
        }
        tail=new_tail;  // 6
    }
};

int main()
{
    queue<int> q;

    // Test 2: Push an element and check if it can be popped
    q.push(1);
    auto res = q.try_pop();
    assert(res && *res == 1);

    // Test 4: Try to pop from an empty queue and check if it returns a null shared_ptr
    res = q.try_pop();
    assert(!res);

    // Test 5: Push multiple elements and check if they can be popped in the correct order
    for (int i = 0; i < 10; ++i)
    {
        q.push(i);
    }
    for (int i = 0; i < 10; ++i)
    {
        res = q.try_pop();
        assert(res && *res == i);
    }

    return 0;
}