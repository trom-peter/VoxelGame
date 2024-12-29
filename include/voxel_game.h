#pragma once

#include "renderer.h"
#include "window.h"
#include "input_handler.h"
#include "player.h"
#include "world.h"
#include "seed_generator.h"

class VoxelGame {

public:
	VoxelGame();

	int init();
	void run();
	int quit();

private:
	void generateChunks();
	void drawChunks();

	Window* window;
	Renderer* renderer;
	Shader* shader;
	VertexArray* vao;
	InputHandler* inputHandler;
	Player* player;
	World* world;
	bool running;
	glm::ivec2 currentChunk;
};