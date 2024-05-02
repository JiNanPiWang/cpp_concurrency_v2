//
// Created by Samsara on 2024/5/2.
// File name: 4.cpp
// Description: 
//
#include <iostream>
#include <future>
#include <thread>
#include <unistd.h>

using namespace std;

int getValueInFuture(int x)
{
    sleep(2);
    return x;
}

int main()
{
    future<int> result = async(getValueInFuture, 2);
    cout << result.get() << endl;
    return 0;
}
