#include "world.h"

World::World(unsigned short renderDistance, glm::vec3 skyColor, glm::vec3 sunColor, glm::vec3 sunDirection) : 
	renderDistance(renderDistance), skyColor(skyColor), sunColor(sunColor), sunDirection(sunDirection) {}

unsigned short World::getRenderDistance() {
	return renderDistance;
}

glm::vec3 World::getSkyColor() {
	return skyColor;
}

glm::vec3 World::getSunColor() {
	return sunColor;
}

glm::vec3 World::getSunDirection() {
	return sunDirection;
}

std::unordered_map<std::pair<int, int>, std::unique_ptr<Chunk>, pair_hash>* World::getChunks() {
	return &chunks;
}