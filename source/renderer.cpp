#include "renderer.h"

void GLAPIENTRY openGLDebugCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam) {
    std::cout << "[OpenGL Error " << id << "] " << message << std::endl;
}

float Renderer::NEAR;
float Renderer::FAR;
Renderer::Renderer(float nearPlane, float farPlane) {
    Renderer::NEAR = nearPlane;
    Renderer::FAR = farPlane;
}

bool Renderer::init() {
    GLenum err = glewInit();
    if (err != GLEW_OK) {
        std::cout << "Error: " << glewGetErrorString(err) << std::endl;
        return false;
    }
    std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl;

    #ifdef _DEBUG
    setupGLDebugCallback();
    #endif

    glEnable(GL_CULL_FACE);
    glFrontFace(GL_CCW);  // Specify that the front faces are wound CCW    
    glCullFace(GL_BACK);  // Cull back faces
    glEnable(GL_DEPTH_TEST);

    glEnable(GL_MULTISAMPLE);

    return true;
}

void Renderer::draw(Chunk* chunk) {
    Mesh* mesh = chunk->getMesh();
    mesh->vao->addVertexBuffer(*mesh->vertexBuffer); 
    mesh->vertexBuffer->bind();
    mesh->indexBuffer->bind();
    mesh->shader->bind();
    mesh->shader->setUniform2fv("u_chunkPosition", 1, chunk->getPosition());
    glDrawElements(GL_TRIANGLES, mesh->numIndices, GL_UNSIGNED_INT, 0);
    mesh->indexBuffer->unbind();
    mesh->vertexBuffer->unbind();
    mesh->vao->unbind();
}

void Renderer::drawInstanced(VertexArray& vao, IndexBuffer& ib, Shader& shader, int numInstances) {
    shader.bind();
    vao.bind();
    ib.bind();
    glDrawElementsInstanced(GL_TRIANGLES, ib.getNumIndices(), GL_UNSIGNED_INT, 0, numInstances);
}

void Renderer::clear() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::setupGLDebugCallback() {
    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    glDebugMessageCallback(openGLDebugCallback, 0);
}

void Renderer::setClearColor(GLclampf r, GLclampf g, GLclampf b) {
    glClearColor(r, g, b, 1.0f);
}
