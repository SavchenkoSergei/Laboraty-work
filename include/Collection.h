#pragma once

#include <vector>
#include <iostream>
#include <algorithm>

template <typename T>
class Collection {
private:
    std::vector<T> items;

public:
    Collection() = default;

    void add(const T& item) { 
        items.push_back(item); 
    }
    void add(T&& item) { 
        items.push_back(std::move(item)); 
    }

    template <typename Predicate>
    bool removeIf(Predicate predicate) {
        auto initialSize = items.size();
        std::erase_if(items, predicate);
        return items.size() < initialSize;
    }

    const T& getAt(size_t index) const {
        return items[index]; 
    }

    template <typename Predicate>
    T* find(Predicate predicate) {
        for (auto& item : items) {
            if (predicate(item)) return &item;
        }
        return nullptr;
    }

    template <typename Predicate>
    const T* find(Predicate predicate) const {
        for (const auto& item : items) {
            if (predicate(item)) return &item;
        }
        return nullptr;
    }

    template <typename Compare>
    void sort(Compare comp) {
        std::ranges::sort(items, comp);
    }

    size_t size() const { 
        return items.size(); 
    }

    bool empty() const { 
        return items.empty(); 
    }

    void clear() { 
        items.clear(); 
    }

    void print() const {
        if (items.empty()) {
            std::cout << "Пусто...\n";
            return;
        }
        for (size_t i = 0; i < items.size(); ++i) {
            std::cout << (i + 1) << ". " << items[i] << "\n";
        }
    }
};

template <typename T, typename Predicate>
size_t countMatches(const Collection<T>& collection, Predicate predicate) {
    size_t count = 0;
    for (size_t i = 0; i < collection.size(); ++i) {
        if (predicate(collection.getAt(i))) {
            ++count;
        }
    }
    return count;
}