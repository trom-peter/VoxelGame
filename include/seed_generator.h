#pragma once

#include <random>

struct SeedGenerator {
    static int getRandomInt() {
        static std::random_device seed;
        static std::mt19937 gen{ seed() };
        std::uniform_int_distribution<> dist{ std::numeric_limits<int>::min(), std::numeric_limits<int>::max() };
        return dist(gen);
    }
};
