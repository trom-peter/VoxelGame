#include "world.h"
#include <random>
#include <limits>

World::World(unsigned short renderDistance, glm::vec3 skyColor, glm::vec3 sunColor, glm::vec3 sunDirection, int seed) : 
	renderDistance(renderDistance), skyColor(skyColor), sunColor(sunColor), sunDirection(sunDirection), seed(seed) {}

World::~World() {}

int World::getSeed() {
	return seed;
}

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