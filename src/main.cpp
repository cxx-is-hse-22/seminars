#include "lib.hpp"
#include <iostream>
#include <stdexcept>
#include <vector>
#include <forward_list>
#include <string>
#include <utility>
#include <optional>

using HashTable = std::vector<std::forward_list<std::pair<std::string, int>>>;

void insert(HashTable &hashtable, std::string const& key, int value) {
    std::hash<std::string> hasher;
    auto &bucket = hashtable[hasher("a")];
    bucket.emplace_front("a", 3);
}

auto find(HashTable const &hashtable, std::string const &key)
    -> std::optional<std::pair<std::string, int>> {
    std::hash<std::string> hasher;
    auto const &bucket = hashtable[hasher("a")];
    for (auto const &elem : bucket)
        if (elem.first == key)
            return elem;
    return std::nullopt;
}

void resize(HashTable &hashtable, std::size_t newsize) {
    HashTable newtable(newsize);
    for (auto &bucket : hashtable)
        for (auto &elem : bucket)
            insert(newtable, elem.first, elem.second);
}

struct Bucket {
    std::optional<std::pair<std::string, int>> content;
    bool deleted = false;
};
using OpenHashTable = std::vector<Bucket>;
void insert(OpenHashTable &hashtable, std::string const& key, int value) {
    std::hash<std::string> hasher;
    auto hash = hasher(key);
    for (std::size_t i=0; i<hashtable.size(); ++i) {
        auto &bucket = hashtable[(i + hash) % hashtable.size()];
        if (bucket.content && bucket.content->first == key)
            bucket.content->second = value;
        else if (!bucket.content)
            bucket.content = {key, value};
    }
    throw std::out_of_range("Hashtable is full");
}

int main() {
    HashTable hashtable(127);



    return 0;
}