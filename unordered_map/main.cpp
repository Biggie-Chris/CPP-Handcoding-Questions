#include <iostream>
#include "unordered_map.hpp"
#include <memory>


int main() {
    MyChainingHashMap<int, std::shared_ptr<int>> map;
    map.put(1, std::make_shared<int>(1));
    map.put(2, std::make_shared<int>(2));
    map.put(3, std::make_shared<int>(3));
    std::cout << *map.get(1) << std::endl; // 1
    std::cout << *map.get(2) << std::endl; // 2

    map.put(1, std::make_shared<int>(100));
    std::cout << *map.get(1) << std::endl; // 100

    map.remove(2);
    std::cout << (map.get(2) == nullptr ? "null" : std::to_string(*map.get(2))) << std::endl; // null

    for (const auto key: map.keys()) {
        std::cout << key << " ";
    }
    // [1, 3]（顺序可能不同）
    std::cout << std::endl;

    map.remove(1);
    map.remove(2);
    map.remove(3);
    std::cout << (map.get(1) == nullptr ? "null" : std::to_string(*map.get(1))) << std::endl; // null

    return 0;
}