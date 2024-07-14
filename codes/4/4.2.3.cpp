//
// Created by Samsara on 2024/5/4.
// File name: 4.cpp
// Description:
// 需要执行函数的时候，可以用packaged_task
// 需要手动设置值的时候，可以用promise
// 主要作用是一个异步线程可以给另外一个线程提供信息，详见processData
//
#include <iostream>
#include <future>

using namespace std;

// 生成数据的函数
void generateData(std::promise<int>&& promise) {
    std::this_thread::sleep_for(std::chrono::seconds(1)); // 模拟耗时操作
    int data = 42; // 生成的数据
    std::cout << "Data generated: " << data << std::endl;
    promise.set_value(data); // 将数据设置到promise中
}

// 处理数据的函数
void processData(std::future<int>&& future) {
    int data = future.get(); // 当另外一个异步线程执行好后，我们得到了它的值，我们再开始运行，否则阻塞于此
    std::cout << "Processing data: " << data << std::endl;
    // 模拟数据处理
    data *= 2;
    std::cout << "Data processed: " << data << std::endl;
}



void f(std::promise<long long> &p)
{
    p.set_value(100);
}

int main()
{
    std::promise<long long> prom;
    std::future<long long> fu = prom.get_future();
    thread t(f, std::ref(prom));
    auto x = fu.get();
    cout << "future: " << x << endl;
    t.join();


    // 创建promise和future
    std::promise<int> promise;
    std::future<int> future = promise.get_future();

    // 启动线程生成数据
    std::thread producer(generateData, std::move(promise));

    // 启动线程处理数据
    std::thread consumer(processData, std::move(future));

    // 等待两个线程完成
    producer.join();
    consumer.join();


    return 0;
}
