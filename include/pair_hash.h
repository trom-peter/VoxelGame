#ifndef PAIR_HASH_H
#define PAIR_HASH_H

#include <utility>
#include <functional>

struct pair_hash {
    std::size_t operator()(const std::pair<int, int>& p) const {
        return std::hash<int>{}(p.first) ^ (std::hash<int>{}(p.second) << 1);
    }
};

#endif
