//
// Created by Samsara on 2024/6/23.
// File name: 7.cpp
// Description: 
//
#include <iostream>
#include <atomic>
#include <memory>

using namespace std;

template<typename T>
class lock_free_stack
{
private:
    struct node
    {
        shared_ptr<T> data;
        node *next;
        explicit node (const T &x) : data(make_shared<T>(x)) {}
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

    shared_ptr<T> pop()
    {
        auto *old_head = head.load();
        while (old_head != nullptr &&
                !head.compare_exchange_weak(old_head, old_head->next));
        return old_head == nullptr ? shared_ptr<T>() : old_head->data;
    }
};

int main()
{
    lock_free_stack<int> stack;

    // Test push
    stack.push(1);
    stack.push(2);
    stack.push(3);

    // Test pop
    auto value = stack.pop();
    if (value)
        cout << "Popped: " << *value << endl;
    else
        cout << "Stack is empty." << endl;

    value = stack.pop();
    if (value)
        cout << "Popped: " << *value << endl;
    else
        cout << "Stack is empty." << endl;

    value = stack.pop();
    if (value)
        cout << "Popped: " << *value << endl;
    else
        cout << "Stack is empty." << endl;

    // Test pop on empty stack
    value = stack.pop();
    if (value)
        cout << "Popped: " << *value << endl;
    else
        cout << "Stack is empty." << endl;

    return 0;
}