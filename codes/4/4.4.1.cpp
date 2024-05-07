//
// Created by Samsara on 2024/5/7.
// File name: 4.cpp
// Description:
//
#include <iostream>
#include <list>
#include <algorithm>

using namespace std;

template<typename T>
std::list<T> sequential_quick_sort(std::list<T> input)
{
    // 如果输入列表为空，则直接返回
    if(input.empty())
    {
        return input;
    }
    // 创建一个新的列表 result 来存放排序后的结果
    std::list<T> result;
    // 将输入列表的第一个元素移到 result 列表的开头
    result.splice(result.begin(), input, input.begin());  // 1
    // 设置枢轴值为 result 列表的第一个元素
    T const& pivot = *result.begin();  // 2

    // 将输入列表中的元素根据枢轴值进行分区
    auto divide_point = std::partition(input.begin(), input.end(),
                                       [&](T const& t){ return t < pivot; });  // 3

    // 创建一个新的列表 lower_part 来存放小于枢轴值的元素
    std::list<T> lower_part;
    // 将输入列表中小于枢轴值的元素移到 lower_part 列表的末尾
    lower_part.splice(lower_part.end(), input, input.begin(), divide_point);  // 4
    // 对 lower_part 列表进行递归排序
    auto new_lower = sequential_quick_sort(std::move(lower_part));  // 5
    // 对剩余的元素（大于等于枢轴值的元素）进行递归排序
    auto new_higher = sequential_quick_sort(std::move(input));  // 6

    // 将 new_higher 列表的元素移到 result 列表的末尾
    result.splice(result.end(), new_higher);  // 7
    // 将 new_lower 列表的元素移到 result 列表的开头
    result.splice(result.begin(), new_lower);  // 8
    // 返回排序后的结果列表
    return result;
}


int main()
{

    return 0;
}