#pragma once

#include <vector>

#include "vertex.h"
#include "shader.h"
#include "vertex_buffer.h"
#include "index_buffer.h"
#include "vertex_array.h"

struct Shader;
class VertexArray;
struct VertexBuffer;

struct Mesh {
    Mesh(std::vector<Vertex>& vertices, size_t numVertices, std::vector<uint32_t>& indices, size_t numIndices, Shader* shader, VertexArray* vao);
    ~Mesh();

    VertexArray* vao;
    VertexBuffer* vertexBuffer;
    IndexBuffer* indexBuffer;
    Shader* shader;
    uint64_t numIndices = 0;
};