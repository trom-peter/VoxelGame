#pragma once

#include "fps_camera.h"
#include "floating_camera.h"
#include "fps_camera_controller.h"
#include "floating_camera_controller.h"
#include "input_handler.h"
#include "window.h"
#include "box_collision.h"

class Player {

public:
	Player(float speed, float mouseSensitivity, float jumpHeight, float fov, Window& window, World* world);
	void moveForward();
	void moveSideways();
	void jump();
	void update(InputHandler* inputs, float delta);

	float getSpeed();
	float getFov();
	float getJumpHeight();
	float getMouseSensitivity();
	glm::vec3 getPosition();
	glm::vec2 getChunkPosition();
	std::vector<Block> getSurroundingBlocks();

	FPSCamera* getCamera();
	BoxCollision* getCollisionBox();

private:
	World* world;
	FloatingCamera* camera;
	FloatingCameraController* controller;
	BoxCollision* collisionBox;
};