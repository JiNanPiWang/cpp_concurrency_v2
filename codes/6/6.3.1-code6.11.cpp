//
// Created by Samsara on 2024/6/10.
// File name: 6.cpp
// Description:
//
#include <iostream>
#include <thread>
#include <mutex>
#include <shared_mutex>
#include <list>
#include <algorithm>
#include <vector>
#include <cassert>


using namespace std;

template<typename key_type, typename value_type, typename hash_type=std::hash<key_type>>
class threadsafe_lookup_table
{
private:
    class basic_bucket
    {
    private:
        using bucket_item_type = pair<key_type, value_type>;
        std::list<bucket_item_type> data;
        mutable shared_mutex bucket_lock;

        auto find_entry_for(const key_type &key) -> decltype(data.begin())
        {
            return find_if(data.begin(), data.end(),
                           [&](const auto &item) -> bool
                           {
                               return item.first == key;
                           });
        }

    public:
        value_type value_for(const key_type &key, const value_type &default_value)
        {
            shared_lock<shared_mutex> lk(bucket_lock);
            const auto it = find_entry_for(key);
            if (it != data.end())
                return it->second;
            else
                return default_value;
        }

        void add_or_update_mapping(const key_type &key, const value_type &value)
        {
            unique_lock<shared_mutex> lk(bucket_lock);
            const auto it = find_entry_for(key);
            if (it != data.end())
                it->second = value;
            else
                data.push_back({key, value});
        }

        void remove_mapping(const key_type &key)
        {
            unique_lock<shared_mutex> lk(bucket_lock);
            const auto it = find_entry_for(key);
            if (it != data.end())
                data.erase(it);
        }
    };

    vector<unique_ptr<basic_bucket>> buckets;

    auto &get_bucket(const key_type &key) // 返回bucket的引用
    {
        const auto &bucket_idx = hash_type()(key) % buckets.size();
        return *buckets[bucket_idx];
    }

public:
    threadsafe_lookup_table(threadsafe_lookup_table const &other) = delete;

    threadsafe_lookup_table &operator=(threadsafe_lookup_table const &other) = delete;

    explicit threadsafe_lookup_table(int num_buckets = 19) : buckets(num_buckets)
    {
        for (int i = 0; i < num_buckets; ++i)
        {
            buckets[i] = make_unique<basic_bucket>();
        }
    }

    value_type value_for(const key_type &key, const value_type &default_value)
    {
        return get_bucket(key).value_for(key, default_value);
    }

    void add_or_update_mapping(const key_type &key, const value_type &value)
    {
        return get_bucket(key).add_or_update_mapping(key, value);
    }

    void remove_mapping(const key_type &key)
    {
        return get_bucket(key).remove_mapping(key);
    }
};

int main()
{
    // 创建一个 threadsafe_lookup_table 对象
    threadsafe_lookup_table<int, std::string> table;

    // 测试 value_for() 函数
    table.add_or_update_mapping(1, "one");
    assert(table.value_for(1, "default") == "one"); // 1 存在于表中，返回 "one"
    assert(table.value_for(2, "default") == "default"); // 2 不在表中，返回 "default"

    // 测试 add_or_update_mapping() 函数
    table.add_or_update_mapping(2, "two");
    assert(table.value_for(2, "default") == "two"); // 2 存在于表中，返回 "two"

    // 测试 remove_mapping() 函数
    table.remove_mapping(2);
    assert(table.value_for(2, "default") == "default"); // 2 已被删除，返回 "default"

    // 添加更多的测试用例...

    std::cout << "All tests passed successfully!" << std::endl;

    return 0;
}