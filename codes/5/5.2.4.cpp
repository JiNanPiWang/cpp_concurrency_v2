//
// Created by Samsara on 2024/5/14.
// File name: 5.cpp
// Description: 
//
#include <iostream>
#include <thread>
#include <atomic>

using namespace std;

int main()
{
    // test fetch_add
    int arr[5] = {1, 2, 3, 4, 5};
    atomic<int*> ptr(arr);
    int *oldptr = ptr.fetch_add(1);
    cout << "fetch_add for ptr: " << endl;
    cout << "oldptr: " << *oldptr << endl; // "oldptr: 1"
    cout << "ptr: " << *ptr << endl; // "ptr: 2"
    return 0;
}