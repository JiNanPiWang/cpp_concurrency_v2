//
// Created by Samsara on 2024/6/22.
// File name: 7.cpp
// Description: 
//
#include <iostream>
#include <atomic>

using namespace std;

template<typename T>
class lock_free_stack
{
private:
    struct node
    {
        T data;
        node *next;
        explicit node (const T &x) : data(x) {}
    };
    atomic<node*> head;

public:
    void push(const T &data)
    {
        auto *new_node = new node(data);
        new_node->next = head.load();
        // 将 head 的当前值与 new_node->next 进行比较。
        // 如果相等，则将 head 更新为 new_node，并返回 true。
        // 如果不相等，则将 head 的当前值更新到 new_node->next 中，并返回 false。
        while (!head.compare_exchange_weak(new_node->next, new_node));
    }
};

int main()
{

    return 0;
}
