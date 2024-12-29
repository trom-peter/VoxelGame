#pragma once

#include "glm/glm.hpp"

struct BoxCollision {

	glm::vec3 position;
	float width; 
	float height;
	float length;

	BoxCollision(glm::vec3 pos, float w, float h, float l) : position(pos), width(w), height(h), length(l) {}

    bool overlapsWith(const BoxCollision& other) {
        if (position.x + width < other.position.x || other.position.x + other.width < position.x) return false;
        if (position.y + height < other.position.y || other.position.y + other.height < position.y) return false;
        if (position.z + length < other.position.z || other.position.z + other.length < position.z) return false;
        #ifdef DEBUG
        std::cout << "COLLISION WITH OBJECTS: (" << position.x << "," << position.y << "," << position.z
            << "; " << other.position.x << "," << other.position.y << "," << other.position.z << ")" << std::endl;
        #endif
        return true;
    }
};