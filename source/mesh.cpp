#include "mesh.h"

Mesh::Mesh(std::vector<Vertex>& vertices, size_t numVertices, std::vector<uint32_t>& indices, size_t numIndices, Shader* shader, VertexArray* vao) {
    this->shader = shader;
    this->numIndices = numIndices;
    this->vao = vao;
    vertexBuffer = new VertexBuffer(vertices.data(), numVertices, sizeof(vertices[0]));
    indexBuffer = new IndexBuffer(indices.data(), numIndices, sizeof(indices[0]));
    vao->bind();
    //vao->addVertexBuffer(*vertexBuffer);
    indexBuffer->bind();
}

Mesh::~Mesh() {
    delete vertexBuffer;
    delete indexBuffer;
}