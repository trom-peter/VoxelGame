#pragma once

#include <unordered_map>
#include "chunk.h"
#include "pair_hash.h"

class Chunk;

class World {

public:
	World(unsigned short renderDistance, glm::vec3 skyColor, glm::vec3 sunColor, glm::vec3 sunDirection);
	~World();

	unsigned short getRenderDistance();
	glm::vec3 getSkyColor();
	glm::vec3 getSunColor();
	glm::vec3 getSunDirection();
	std::unordered_map<std::pair<int, int>, std::unique_ptr<Chunk>, pair_hash>* getChunks();

private:
	std::unordered_map<std::pair<int, int>, std::unique_ptr<Chunk>, pair_hash> chunks;
	unsigned short renderDistance;
	glm::vec3 skyColor;
	glm::vec3 sunColor;
	glm::vec3 sunDirection;
};