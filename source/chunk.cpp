#include "chunk.h"
#define STB_PERLIN_IMPLEMENTATION
#include "stb_perlin.h"

#include <iostream>

Chunk::Chunk(glm::ivec2 position, World& world) : position(position), world(world) {}

Chunk::~Chunk() {
    if (mesh) {
        delete mesh;
        mesh = nullptr;
    }
}

BlockType Chunk::getBlock(int x, int y, int z) {
    float scale = 0.04f;  // Skalierungsfaktor der noisemap
    int baseHeight = 50; // base höhe
    int amplitude = 50;  // maximaler unterschied zur base höhe

    int worldX = x + position.x * CHUNK_WIDTH;
    int worldY = z + position.y * CHUNK_LENGTH;

    float noiseValue = stb_perlin_noise3(worldX * scale, worldY * scale, 0, 0, 0, 0);

    int groundHeight = baseHeight + static_cast<int>(noiseValue * amplitude);

    if (y <= groundHeight - 3) return BlockType::BLOCK_STONE;
    if (y <= groundHeight - 1) return BlockType::BLOCK_DIRT;
    if (y <= groundHeight) return BlockType::BLOCK_GRASS;
    else return BlockType::BLOCK_AIR;
}

void Chunk::generateChunk() {
    for (unsigned short x = 0; x < CHUNK_WIDTH; x++) {
		for (unsigned short y = 0; y < CHUNK_HEIGHT; y++) {
			for (unsigned short z = 0; z < CHUNK_LENGTH; z++) {
				blocks[x][y][z] = getBlock(x, y, z);
			}
		}
	}
}

void Chunk::generateMesh(Shader& shader, VertexArray& vao) {
    std::vector<Vertex> vertices;
    std::vector<uint32_t> indices;
    for (unsigned short x = 0; x < CHUNK_WIDTH; x++) {
        for (unsigned short y = 0; y < CHUNK_HEIGHT; y++) {
            for (unsigned short z = 0; z < CHUNK_LENGTH; z++) {

                glm::vec3 blockPosition(x, y, z);

                if (isAirAt(x, y, z)) continue;
                
                // 1. front face (+z)
                if (z == CHUNK_SIZE - 1 && 
                        world.getChunks()->count({ position.x, position.y + 1 }) == 1 &&
                        (*world.getChunks())[{position.x, position.y + 1}]->isAirAt(x, y, 0)) {
                    addFace(vertices, indices, blockPosition, FaceDirection::FRONT);
                }
                else if (z != CHUNK_SIZE - 1 && isAirAt(x, y, z + 1)) {
                    addFace(vertices, indices, blockPosition, FaceDirection::FRONT);
                }

                // 2. back face (-z)
                if (z == 0 && 
                        world.getChunks()->count({ position.x, position.y - 1 }) == 1 &&
                        (*world.getChunks())[{position.x, position.y - 1}]->isAirAt(x, y, Chunk::CHUNK_SIZE - 1)) {
                    addFace(vertices, indices, blockPosition, FaceDirection::BACK);
                }
                else if (z != 0 && isAirAt(x, y, z - 1)){
                    addFace(vertices, indices, blockPosition, FaceDirection::BACK);
                }

				// 3. left face (-x)
				if (x == 0 &&
                        world.getChunks()->count({position.x - 1, position.y}) == 1 && //chunkAt() != null && chunkAt()->isAirAt(). dann chunk lieber als variable?
                        (*world.getChunks())[{position.x - 1, position.y}]->isAirAt(Chunk::CHUNK_SIZE - 1, y, z)) {
					addFace(vertices, indices, blockPosition, FaceDirection::LEFT);
				}
				else if (x != 0 && isAirAt(x - 1, y, z)) {
					addFace(vertices, indices, blockPosition, FaceDirection::LEFT);
				}

                // 4. right face (+x)
                if (x == CHUNK_WIDTH - 1 && 
                        world.getChunks()->count({position.x + 1, position.y}) == 1 &&
                        (*world.getChunks())[{position.x + 1, position.y}]->isAirAt(0, y, z)) {
                    addFace(vertices, indices, blockPosition, FaceDirection::RIGHT);
                }
                else if (x != CHUNK_WIDTH - 1 && isAirAt(x + 1, y, z)) {
                    addFace(vertices, indices, blockPosition, FaceDirection::RIGHT);
                }

                // 5. top face (+y)
                if (y == CHUNK_HEIGHT - 1 || isAirAt(x, y + 1, z)) {
                    addFace(vertices, indices, blockPosition, FaceDirection::TOP);
                }

                // 6. bottom face (-y)
                if (y == 0 || isAirAt(x, y - 1, z)) {
                    addFace(vertices, indices, blockPosition, FaceDirection::BOTTOM);
                }
            }
        }
    }
    this->mesh = new Mesh(vertices, vertices.size(), indices, indices.size(), &shader, &vao);
}

void Chunk::addFace(std::vector<Vertex>& vertices, std::vector<uint32_t>& indices, glm::vec3 position, FaceDirection dir) {
    size_t startIndex = vertices.size();

    std::array<Vertex, 4> faceVertices;
    std::array<uint32_t, 6> faceIndices = Vertex::getRectIndices();

    glm::vec3 normal;

    int x = position.x;
    int y = position.y;
    int z = position.z;

    //set vertices based on facedirection
    switch (dir) {
        case FaceDirection::FRONT:
            normal = glm::vec3(0.0f, 0.0f, 1.0f);

            faceVertices[0] = Vertex(glm::vec3(-0.5f, -0.5f, 0.5f) + position, glm::ivec2(0, 0), normal, 
                Vertex::calcVertexAO(!isAirAt(x - 1, y, z + 1), !isAirAt(x, y - 1, z + 1), !isAirAt(x - 1, y - 1, z + 1)));

            faceVertices[1] = Vertex(glm::vec3(0.5f, -0.5f, 0.5f) + position, glm::ivec2(1, 0), normal, 
                Vertex::calcVertexAO(!isAirAt(x + 1, y, z + 1), !isAirAt(x, y - 1, z + 1), !isAirAt(x + 1, y - 1, z + 1)));

            faceVertices[2] = Vertex(glm::vec3(0.5f, 0.5f, 0.5f) + position, glm::ivec2(1, 1), normal, 
                Vertex::calcVertexAO(!isAirAt(x + 1, y, z + 1), !isAirAt(x, y + 1, z + 1), !isAirAt(x + 1, y + 1, z + 1)));

            faceVertices[3] = Vertex(glm::vec3(-0.5f, 0.5f, 0.5f) + position, glm::ivec2(0, 1), normal, 
                Vertex::calcVertexAO(!isAirAt(x - 1, y, z + 1), !isAirAt(x, y + 1, z + 1), !isAirAt(x - 1, y + 1, z + 1)));
            break;

        case FaceDirection::BACK:
            normal = glm::vec3(0.0f, 0.0f, -1.0f);

            faceVertices[0] = Vertex(glm::vec3(-0.5f, -0.5f, -0.5f) + position, glm::ivec2(0, 0), normal, 
                Vertex::calcVertexAO(!isAirAt(x - 1, y, z - 1), !isAirAt(x, y - 1, z - 1), !isAirAt(x - 1, y - 1, z - 1)));

            faceVertices[1] = Vertex(glm::vec3(-0.5f, 0.5f, -0.5f) + position, glm::ivec2(0, 1), normal, 
                Vertex::calcVertexAO(!isAirAt(x - 1, y, z - 1), !isAirAt(x, y + 1, z - 1), !isAirAt(x - 1, y + 1, z - 1)));

            faceVertices[2] = Vertex(glm::vec3(0.5f, 0.5f, -0.5f) + position, glm::ivec2(1, 1), normal, 
                Vertex::calcVertexAO(!isAirAt(x + 1, y, z - 1), !isAirAt(x, y + 1, z - 1), !isAirAt(x + 1, y + 1, z - 1)));

            faceVertices[3] = Vertex(glm::vec3(0.5f, -0.5f, -0.5f) + position, glm::ivec2(1, 0), normal, 
                Vertex::calcVertexAO(!isAirAt(x + 1, y, z - 1), !isAirAt(x, y - 1, z - 1), !isAirAt(x + 1, y - 1, z - 1)));
            break;

        case FaceDirection::LEFT:
            normal = glm::vec3(-1.0f, 0.0f, 0.0f);

            faceVertices[0] = Vertex(glm::vec3(-0.5f, -0.5f, 0.5f) + position, glm::ivec2(0, 0), normal, 
                Vertex::calcVertexAO(!isAirAt(x - 1, y - 1, z), !isAirAt(x - 1, y, z + 1), !isAirAt(x - 1, y - 1, z + 1)));

            faceVertices[1] = Vertex(glm::vec3(-0.5f, 0.5f, 0.5f) + position, glm::ivec2(0, 1), normal, 
                Vertex::calcVertexAO(!isAirAt(x - 1, y + 1, z), !isAirAt(x - 1, y, z + 1), !isAirAt(x - 1, y + 1, z + 1)));

            faceVertices[2] = Vertex(glm::vec3(-0.5f, 0.5f, -0.5f) + position, glm::ivec2(1, 1), normal, 
                Vertex::calcVertexAO(!isAirAt(x - 1, y + 1, z), !isAirAt(x - 1, y, z - 1), !isAirAt(x - 1, y + 1, z - 1)));

            faceVertices[3] = Vertex(glm::vec3(-0.5f, -0.5f, -0.5f) + position, glm::ivec2(1, 0), normal, 
                Vertex::calcVertexAO(!isAirAt(x - 1, y - 1, z), !isAirAt(x - 1, y, z - 1), !isAirAt(x - 1, y - 1, z - 1)));
            break;

        case FaceDirection::RIGHT:
            normal = glm::vec3(1.0f, 0.0f, 0.0f);

            faceVertices[0] = Vertex(glm::vec3(0.5f, -0.5f, -0.5f) + position, glm::ivec2(0, 0), normal, 
                Vertex::calcVertexAO(!isAirAt(x + 1, y - 1, z), !isAirAt(x + 1, y, z - 1), !isAirAt(x + 1, y - 1, z - 1)));

            faceVertices[1] = Vertex(glm::vec3(0.5f, 0.5f, -0.5f) + position, glm::ivec2(0, 1), normal, 
                Vertex::calcVertexAO(!isAirAt(x + 1, y + 1, z), !isAirAt(x + 1, y, z - 1), !isAirAt(x + 1, y + 1, z - 1)));

            faceVertices[2] = Vertex(glm::vec3(0.5f, 0.5f, 0.5f) + position, glm::ivec2(1, 1), normal, 
                Vertex::calcVertexAO(!isAirAt(x + 1, y + 1, z), !isAirAt(x + 1, y, z + 1), !isAirAt(x + 1, y + 1, z + 1)));

            faceVertices[3] = Vertex(glm::vec3(0.5f, -0.5f, 0.5f) + position, glm::ivec2(1, 0), normal, 
                Vertex::calcVertexAO(!isAirAt(x + 1, y - 1, z), !isAirAt(x + 1, y, z + 1), !isAirAt(x + 1, y - 1, z + 1)));
            break;

        case FaceDirection::TOP:
            normal = glm::vec3(0.0f, 1.0f, 0.0f);

            faceVertices[0] = Vertex(glm::vec3(-0.5f, 0.5f, -0.5f) + position, glm::ivec2(0, 0), normal, 
                Vertex::calcVertexAO(!isAirAt(x - 1, y + 1, z), !isAirAt(x, y + 1, z - 1), !isAirAt(x - 1, y + 1, z - 1)));

            faceVertices[1] = Vertex(glm::vec3(-0.5f, 0.5f, 0.5f) + position, glm::ivec2(0, 1), normal, 
                Vertex::calcVertexAO(!isAirAt(x - 1, y + 1, z), !isAirAt(x, y + 1, z + 1), !isAirAt(x - 1, y + 1, z + 1)));

            faceVertices[2] = Vertex(glm::vec3(0.5f, 0.5f, 0.5f) + position, glm::ivec2(1, 1), normal, 
                Vertex::calcVertexAO(!isAirAt(x + 1, y + 1, z), !isAirAt(x, y + 1, z + 1), !isAirAt(x + 1, y + 1, z + 1)));

            faceVertices[3] = Vertex(glm::vec3(0.5f, 0.5f, -0.5f) + position, glm::ivec2(1, 0), normal, 
                Vertex::calcVertexAO(!isAirAt(x + 1, y + 1, z), !isAirAt(x, y + 1, z - 1), !isAirAt(x + 1, y + 1, z - 1)));
            break;

        case FaceDirection::BOTTOM:
            normal = glm::vec3(0.0f, -1.0f, 0.0f);

            faceVertices[0] = Vertex(glm::vec3(0.5f, -0.5f, -0.5f) + position, glm::ivec2(1, 0), normal, 
                Vertex::calcVertexAO(!isAirAt(x + 1, y - 1, z), !isAirAt(x, y - 1, z - 1), !isAirAt(x + 1, y - 1, z - 1)));

            faceVertices[1] = Vertex(glm::vec3(0.5f, -0.5f, 0.5f) + position, glm::ivec2(1, 1), normal, 
                Vertex::calcVertexAO(!isAirAt(x + 1, y - 1, z), !isAirAt(x, y - 1, z + 1), !isAirAt(x + 1, y - 1, z + 1)));

            faceVertices[2] = Vertex(glm::vec3(-0.5f, -0.5f, 0.5f) + position, glm::ivec2(0, 1), normal, 
                Vertex::calcVertexAO(!isAirAt(x - 1, y - 1, z), !isAirAt(x, y - 1, z + 1), !isAirAt(x - 1, y - 1, z + 1)));

            faceVertices[3] = Vertex(glm::vec3(-0.5f, -0.5f, -0.5f) + position, glm::ivec2(0, 0), normal, 
                Vertex::calcVertexAO(!isAirAt(x - 1, y - 1, z), !isAirAt(x, y - 1, z - 1), !isAirAt(x - 1, y - 1, z - 1)));
            break;
    }

    // add vertices with correct texture id in atlas
    for (Vertex& vertex : faceVertices) {
        vertex.texId = Block::indexOf(blocks[position.x][position.y][position.z], dir);
        vertices.push_back(vertex);
    }


    // add indices
    for (auto& index : faceIndices) {
        indices.push_back(startIndex + index);
    }
}

glm::ivec2 Chunk::getPosition() {
    return position;
}

Mesh* Chunk::getMesh() {
    return mesh;
}

glm::ivec2 Chunk::toChunkPosition(glm::vec3 worldPosition) {
    glm::vec2 chunkPosition(worldPosition.x / CHUNK_WIDTH, worldPosition.z / CHUNK_LENGTH);
    if (chunkPosition.x != abs(chunkPosition.x)) chunkPosition.x -= 1;
    if (chunkPosition.y != abs(chunkPosition.y)) chunkPosition.y -= 1;
    return chunkPosition;
}

bool Chunk::isAirAt(int x, int y, int z) {
    if (x == -1)                      return (*world.getChunks())[{position.x - 1, position.y}]->isAirAt(CHUNK_SIZE - 1, y, z);
    if (x == CHUNK_SIZE)              return (*world.getChunks())[{position.x + 1, position.y}]->isAirAt(0, y, z);
    if (y == -1 || y == CHUNK_HEIGHT) return true;
    if (z == -1)                      return (*world.getChunks())[{position.x, position.y - 1}]->isAirAt(x, y, CHUNK_SIZE - 1);
    if (z == CHUNK_SIZE)              return (*world.getChunks())[{position.x, position.y + 1}]->isAirAt(x, y, 0);

    return blocks[x][y][z] == BlockType::BLOCK_AIR;
}

bool Chunk::isChunkAt(int x, int y) {
    return world.getChunks()->count({x, y}) == 1; //KÖNNTE CHUNK ODER NULL RETURNEN?
}

bool Chunk::isInDistance(int posX, int posY, unsigned short distance) {
    return (std::abs(this->position.x - posX) <= distance &&
        std::abs(this->position.y - posY) <= distance);
}

