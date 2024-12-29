#pragma once
#include "block.h"
#include "vertex.h"
#include "mesh.h"
#include "pair_hash.h"
#include "world.h"

struct Shader;
struct Mesh;
class VertexArray;
class World;

class Chunk {
public:
	Chunk(glm::ivec2 position, World& world);
	~Chunk();

	static const unsigned short CHUNK_SIZE = 16;
	static const unsigned short CHUNK_HEIGHT = 100;
	static const unsigned short CHUNK_WIDTH = CHUNK_SIZE;
	static const unsigned short CHUNK_LENGTH = CHUNK_SIZE;

	static glm::ivec2 toChunkPosition(glm::vec3 worldPosition);

	void generateChunk();
	void generateMesh(Shader* shader, VertexArray* vao);
	bool isInDistance(int posX, int posY, unsigned short distance);
	glm::ivec2 getPosition();
	Mesh* getMesh();

private:
	void addFace(std::vector<Vertex>& vertices, std::vector<uint32_t>& indices, glm::vec3 position, FaceDirection dir);
	BlockType getBlock(int x, int y, int z);
	bool isAirAt(int x, int y, int z);
	std::array<std::array<std::array<Block, CHUNK_WIDTH>, CHUNK_HEIGHT>, CHUNK_LENGTH> blocks;

	World& world;
	Mesh* mesh = nullptr;
	glm::ivec2 position;
};