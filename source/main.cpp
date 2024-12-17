#include <cmath> 
#include <vector>
#include <memory>

#include "renderer.h"
#include "window.h"
#include "input_handler.h"
#include "shader.h"
#include "vertex_buffer_layout.h"
#include "vertex_array.h"
#include "index_buffer.h"
#include "floating_camera.h"
#include "block.h"
#include "camera_controller.h"
#include "texture_loader.h"
#include "texture_atlas.h"
#include "chunk.h"
#include "pair_hash.h"
#include "world.h"

#pragma comment(lib, "SDL2.lib")
#pragma comment(lib, "glew32s.lib")
#pragma comment(lib, "opengl32.lib")

int main(int argc, char** argv) {
    Window window(2560, 1440, "MINECRAFT");
    Renderer renderer;
    InputHandler inputHandler;

    if (!window.init()) {
        return -1;
    }

    if (!renderer.init()) {
        window.quit();
        return -1;
    }

    World* world = new World(15, glm::vec3(0.8f, 0.8f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.0f, 1.0f, 0.0f));

    renderer.setClearColor(world->getSkyColor().x, world->getSkyColor().y, world->getSkyColor().z);

    TextureAtlas atlas("Textures/tex_atlas.png", 256, 256, 16, 16);
    TextureLoader::loadTexture(atlas.filename, &atlas.id);

    VertexBufferLayout vertexLayout;
    vertexLayout.push<float>(3); //vertex position
    vertexLayout.push<float>(2); //vertex texture uv
    vertexLayout.push<float>(3); //vertex normal
    vertexLayout.push<float>(2); //atlas texture id
    vertexLayout.push<float>(1); //ambient occlusion (0,1,2,3)

    VertexArray vao;
    vao.bind();
    vao.setLayout(&vertexLayout);

    Shader shader("Shaders/basic.vert", "Shaders/basic.frag");
    shader.bind();

    glm::ivec2 lastChunk(0, 0);
    glm::ivec2 currentChunk(0, 0);

    glm::mat4 model = glm::mat4(1.0f);

    FloatingCamera camera(90.0f, window.getWidth(), window.getHeight(), 20.0f, 0.1f);
    camera.translate(glm::vec3(0, 60, 0));
    camera.update();
    CameraController cameraController(camera);

    glm::mat4 modelViewProj = camera.getViewProj() * model;
    shader.setUniformMatrix4fv("u_modelViewProj", 1, GL_FALSE, modelViewProj);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, atlas.id);
    shader.setUniform1i("u_texture_atlas", 0);
    shader.setUniform1i("u_atlasWidth", atlas.atlasWidth);
    shader.setUniform1i("u_atlasHeight", atlas.atlasHeight);
    shader.setUniform1i("u_tileWidth", atlas.tileWidth);
    shader.setUniform1i("u_tileHeight", atlas.tileHeight);
    shader.setUniform3fv("u_sunColor", 1, world->getSunColor());
    shader.setUniform1i("u_chunkWidth", Chunk::CHUNK_WIDTH);
    shader.setUniform1i("u_chunkLength", Chunk::CHUNK_LENGTH);
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    bool running = true;
    while (running) {
        //std::cout << "CHUNK: " << currentChunk.x << ", " << currentChunk.y << std::endl;
        //std::cout << "x:" << camera.getPosition().x << std::endl;
        //std::cout << "y:" << camera.getPosition().y << std::endl;
        //std::cout << "z:" << camera.getPosition().z << std::endl << std::endl;
        inputHandler.handleEvents(running);
        renderer.clear();
        window.increaseTime();

        window.handleInputs(inputHandler);
        if (window.isActive()) cameraController.handleInputs(inputHandler, window.getDelta());

        modelViewProj = camera.getViewProj() * model;
        shader.setUniformMatrix4fv("u_modelViewProj", 1, GL_FALSE, modelViewProj);
        glm::mat4 modelView = camera.getView() * model;
        glm::mat4 invModelView = glm::transpose(glm::inverse(modelView));
        glm::vec4 transformedSunDirection = glm::transpose(glm::inverse(camera.getView())) * glm::vec4(world->getSunDirection(), 1.0f);
        shader.setUniform3fv("u_sunDirection", 1, transformedSunDirection);
        shader.setUniformMatrix4fv("u_invModelView", 1, GL_FALSE, invModelView);

        currentChunk = Chunk::toChunkPosition(camera.getPosition());

        for (int i = -world->getRenderDistance() ; i <= world->getRenderDistance(); i++) {
            for (int j = -world->getRenderDistance(); j <= world->getRenderDistance(); j++) {
                int posX = i + currentChunk.x;
                int posY = j + currentChunk.y;
                if (world->getChunks()->find({posX, posY}) == world->getChunks()->end()) {
                    //chunk is not in cache
                    world->getChunks()->emplace(
                        std::make_pair(posX, posY),
                        std::make_unique<Chunk>(glm::ivec2(posX, posY), *world));
                    world->getChunks()->at({posX, posY})->generateChunk();
                }
            }
        }

        for (auto& kv : *world->getChunks()) {
            if (!kv.second) {
                std::cerr << "Error: Nullptr detected for chunk at position: " << kv.first.first << ", " << kv.first.second << std::endl;
                continue;
            }            
            //generate meshes and render chunks from cache that are in render distance - 1
            if (kv.second->isInDistance(currentChunk.x, currentChunk.y, world->getRenderDistance() - 1)) {
                //std::cout << "render chunk: " << chunk->getPosition().x << ", " << chunk->getPosition().y << std::endl;
                if (kv.second->getMesh() == nullptr) {
                    kv.second->generateMesh(shader, vao);
                }
                renderer.draw(kv.second.get());
            }

            //delete chunk if not in render distance
            else if (!kv.second->isInDistance(currentChunk.x, currentChunk.y, world->getRenderDistance())) {
                world->getChunks()->erase(kv.first);
            }
        }

        lastChunk = currentChunk;

        window.swapBuffers();
        window.updateTime();
    }
    window.quit();
    return 0;
}