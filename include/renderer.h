#pragma once

#include <iostream>
#define GLEW_STATIC
#include <GL/glew.h>
#include "vertex_array.h"
#include "shader.h"
#include "index_buffer.h"
#include "chunk.h"
class Chunk;
class VertexArray;
struct Shader;

void GLAPIENTRY openGLDebugCallback(GLenum source, GLenum type, GLuint id, GLenum severity,
    GLsizei length, const GLchar* message, const void* userParam);

class Renderer {

public:
    Renderer(float nearPlane, float farPlane);

    bool init();

    void draw(Chunk* chunk);

    void clear();

    void drawInstanced(VertexArray& vao, IndexBuffer& ib, Shader& shader, int numInstances);

    void setClearColor(GLclampf r, GLclampf g, GLclampf b);

    static float NEAR;
    static float FAR;
private:
    void setupGLDebugCallback();
};