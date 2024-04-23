//
// Created by Samsara on 2024/4/23.
// File name: 2.2.cpp
// Description: 
//
#include <iostream>
#include <thread>

using namespace std;

void f(int i, std::string const &s);

void oops(int some_param)
{
    char buffer[1024]; // 1
    sprintf(buffer, "%i", some_param);
    // char const *会转为std::string
    // oops结束，buffer指向的内容也会没
    std::thread t(f, 3, buffer); // 2
    t.detach();
}

void oops1(int some_param)
{
    char buffer[1024]; // 1
    sprintf(buffer, "%i", some_param);
    std::thread t(f,3,std::string(buffer));  // 使用std::string，避免悬空指针
    t.detach();
}

int main()
{

    return 0;
}