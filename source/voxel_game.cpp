#include "voxel_game.h"

VoxelGame::VoxelGame() {}

int VoxelGame::init() {
    window = new Window(2560, 1440, "VoxelGame");
    renderer = new Renderer(0.8f, 300.0f);
    inputHandler = new InputHandler();

    if (!window->init()) {
        return -1;
    }

    if (!renderer->init()) {
        window->quit();
        return -1;
    }

    int seed = SeedGenerator::getRandomInt();
    #ifdef DEBUG 
    std::cout << "Generated World Seed: " << seed << std::endl; 
    #endif

    world = new World(15, glm::vec3(0.8f, 0.8f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.0f, 1.0f, 0.0f), seed);
    player = new Player(20.0f, 0.1f, 2.0f, 90.0f, *window, world);

    vao = new VertexArray();
    vao->setupVertexLayout();

    shader = new Shader("Shaders/basic.vert", "Shaders/basic.frag");
    shader->bind();

    renderer->setClearColor(world->getSkyColor().x, world->getSkyColor().y, world->getSkyColor().z);
    renderer->setupTextureAtlas(shader);
    renderer->setupUniforms(shader, world);

    running = true;
    currentChunk = glm::ivec2(0, 0);

    return 0;
}

void VoxelGame::run() {
    while (running) {
        #ifdef DEBUG
        std::cout << "CHUNK: " << currentChunk.x << ", " << currentChunk.y << std::endl;
        std::cout << "x:" << player->getCamera()->getPosition().x << std::endl;
        std::cout << "y:" << player->getCamera()->getPosition().y << std::endl;
        std::cout << "z:" << player->getCamera()->getPosition().z << std::endl << std::endl;
        #endif
        inputHandler->handleEvents(running);

        window->handleInputs(inputHandler);
        if (window->isActive()) player->update(inputHandler, window->getDelta());

        currentChunk = player->getChunkPosition();

        generateChunks();

        renderer->clear();
        renderer->updateUniforms(shader, world, player->getCamera());
        drawChunks();

        window->swapBuffers();
        window->updateTime();
    }
}

int VoxelGame::quit() {
    window->quit();
    delete window;
    delete renderer;
    delete shader;
    delete vao;
    delete inputHandler;
    delete player;
    delete world;
    return 0;
}

void VoxelGame::generateChunks() {
    for (int i = -world->getRenderDistance(); i <= world->getRenderDistance(); i++) {
        for (int j = -world->getRenderDistance(); j <= world->getRenderDistance(); j++) {
            int posX = i + currentChunk.x;
            int posY = j + currentChunk.y;
            if (world->getChunks()->find({ posX, posY }) == world->getChunks()->end()) {
                //chunk is not in cache
                world->getChunks()->emplace(
                    std::make_pair(posX, posY),
                    std::make_unique<Chunk>(glm::ivec2(posX, posY), *world));
                world->getChunks()->at({ posX, posY })->generateChunk();
            }
        }
    }
}

void VoxelGame::drawChunks() {
    std::vector<std::pair<int, int>> chunksToRemove;

    for (auto& kv : *world->getChunks()) {
        if (!kv.second) {
            std::cerr << "Error: Nullptr detected for chunk at position: " << kv.first.first << ", " << kv.first.second << std::endl;
            continue;
        }

        //generate meshes and render chunks from cache that are in render distance - 1
        if (kv.second->isInDistance(currentChunk.x, currentChunk.y, world->getRenderDistance() - 1)) {
            if (kv.second->getMesh() == nullptr) {
                kv.second->generateMesh(shader, vao);
            }
            renderer->draw(kv.second.get());
        }

        //mark chunk for deletion if not in render distance
        else if (!kv.second->isInDistance(currentChunk.x, currentChunk.y, world->getRenderDistance())) {
            chunksToRemove.push_back(kv.first);
        }
    }

    for (const auto& chunkPos : chunksToRemove) {
        world->getChunks()->erase(chunkPos);
    }
}