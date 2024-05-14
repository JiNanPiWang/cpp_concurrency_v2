//
// Created by Samsara on 2024/5/14.
// File name: 5.cpp
// Description: 
//
#include <iostream>
#include <atomic>

using namespace std;

int main()
{
    atomic<int> a(1);
    cout << "fetch_add for int: " << endl;
    cout << "before: " << a.fetch_add(1) << endl; // "1"
    cout << "after: " << a.load() << endl; // "2"

    ++a;
    cout << "++a: " << a.load() << endl; // "3"

    return 0;
}