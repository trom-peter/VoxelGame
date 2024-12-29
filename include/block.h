#pragma once
#include <array>
#include <unordered_map>
#include <cstdint>
#include "glm/glm.hpp"
#include "vertex.h"

enum class BlockType : std::uint8_t {
    BLOCK_DIRT,
    BLOCK_STONE,
    BLOCK_GRASS,
    BLOCK_AIR
};

struct Block {
    Block(BlockType type);
    Block();

    BlockType type;

    static std::unordered_map<BlockType, std::unordered_map<FaceDirection, glm::vec2>> blockIndices;

    static glm::vec2 indexOf(BlockType type, FaceDirection dir);
};
