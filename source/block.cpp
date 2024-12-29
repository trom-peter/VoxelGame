#include "block.h"

std::unordered_map<BlockType, std::unordered_map<FaceDirection, glm::vec2>> Block::blockIndices = {
    { BlockType::BLOCK_DIRT, 
        {
            { FaceDirection::TOP, glm::vec2(2, 15) },
            { FaceDirection::BACK, glm::vec2(2, 15) },
            { FaceDirection::FRONT, glm::vec2(2, 15) },
            { FaceDirection::RIGHT, glm::vec2(2, 15) },
            { FaceDirection::LEFT, glm::vec2(2, 15) },
            { FaceDirection::BOTTOM, glm::vec2(2, 15) }
        }
    },

    { BlockType::BLOCK_STONE,
        {
            { FaceDirection::TOP, glm::vec2(1, 15) },
            { FaceDirection::BOTTOM, glm::vec2(1, 15) },
            { FaceDirection::FRONT, glm::vec2(1, 15) },
            { FaceDirection::RIGHT, glm::vec2(1, 15) },
            { FaceDirection::LEFT, glm::vec2(1, 15) },
            { FaceDirection::BACK, glm::vec2(1, 15) }
        }
    },

    { BlockType::BLOCK_GRASS,
        {   { FaceDirection::TOP, glm::vec2(0, 15) },
            { FaceDirection::BOTTOM, glm::vec2(2, 15) },
            { FaceDirection::FRONT, glm::vec2(3, 15) },
            { FaceDirection::RIGHT, glm::vec2(3, 15) },
            { FaceDirection::LEFT, glm::vec2(3, 15) },
            { FaceDirection::BACK, glm::vec2(3, 15) }
        }
    }
};

Block::Block(BlockType type) : type(type) {}

Block::Block(){}

glm::vec2 Block::indexOf(BlockType type, FaceDirection dir) {
    return blockIndices[type][dir];
}
