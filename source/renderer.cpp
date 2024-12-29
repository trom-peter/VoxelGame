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

void Renderer::setupTextureAtlas(Shader* shader) {
    atlas = new TextureAtlas("Textures/tex_atlas.png", 256, 256, 16, 16);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, atlas->id);
}

void Renderer::setupUniforms(Shader* shader, World* world) {
    shader->setUniform1i("u_texture_atlas", 0);
    shader->setUniform1i("u_atlasWidth", atlas->atlasWidth);
    shader->setUniform1i("u_atlasHeight", atlas->atlasHeight);
    shader->setUniform1i("u_tileWidth", atlas->tileWidth);
    shader->setUniform1i("u_tileHeight", atlas->tileHeight);

    shader->setUniform3fv("u_sunColor", 1, world->getSunColor());
    shader->setUniform1i("u_chunkWidth", Chunk::CHUNK_WIDTH);
    shader->setUniform1i("u_chunkLength", Chunk::CHUNK_LENGTH);
    shader->setUniform1f("u_near", Renderer::NEAR);
    shader->setUniform1f("u_far", Renderer::FAR);
    shader->setUniform3fv("u_skyColor", 1, world->getSkyColor());
}

void Renderer::updateUniforms(Shader* shader, World* world, Camera* camera) {
    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 modelViewProj = camera->getViewProj() * model;
    glm::mat4 modelView = camera->getView() * model;
    glm::mat4 invModelView = glm::transpose(glm::inverse(modelView));
    glm::vec4 transformedSunDirection = glm::transpose(glm::inverse(camera->getView())) * glm::vec4(world->getSunDirection(), 1.0f);

    shader->setUniformMatrix4fv("u_modelViewProj", 1, GL_FALSE, modelViewProj);
    shader->setUniform3fv("u_sunDirection", 1, transformedSunDirection);
    shader->setUniformMatrix4fv("u_invModelView", 1, GL_FALSE, invModelView);
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
