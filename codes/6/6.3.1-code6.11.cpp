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

public:
    // For testing purposes, expose basic_bucket
    basic_bucket bucket;
};

int main()
{
    threadsafe_lookup_table<int, std::string> table;

    // Add or update mappings
    table.bucket.add_or_update_mapping(1, "One");
    table.bucket.add_or_update_mapping(2, "Two");
    table.bucket.add_or_update_mapping(3, "Three");

    // Retrieve values
    std::cout << "Key 1: " << table.bucket.value_for(1, "Not Found") << std::endl;
    std::cout << "Key 2: " << table.bucket.value_for(2, "Not Found") << std::endl;
    std::cout << "Key 3: " << table.bucket.value_for(3, "Not Found") << std::endl;
    std::cout << "Key 4: " << table.bucket.value_for(4, "Not Found") << std::endl;

    // Remove a mapping
    table.bucket.remove_mapping(2);
    std::cout << "Key 2 after removal: " << table.bucket.value_for(2, "Not Found") << std::endl;

    return 0;
}