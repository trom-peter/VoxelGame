#pragma once

#include "glm/glm.hpp"

struct Plane
{
    // unit vector
    glm::vec3 normal = { 0.0f, 1.0f, 0.0f };

    // distance from origin to the nearest point in the plane
    float distance = 0.0f;

};