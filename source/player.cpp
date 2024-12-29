#include "player.h"
#include "chunk.h"

Player::Player(float speed, float mouseSensitivity, float jumpHeight, float fov, Window& window, World* world) : world(world) {
	camera = new FloatingCamera(fov, window.getWidth(), window.getHeight(), speed, mouseSensitivity);
	controller = new FloatingCameraController(*camera);
	collisionBox = new BoxCollision(getPosition(), 0.5f, 0.5f, 2.0f);
	camera->translate(glm::vec3(0.0f, 60.0f, 0.0f));
}

void Player::moveForward() {

}

void Player::moveSideways() {

}

void Player::jump() {

}

void Player::update(InputHandler* inputs, float delta) {
	controller->handleInputs(inputs, delta);
	camera->update();
	collisionBox->position = getPosition();
}

float Player::getSpeed() {
	return camera->getSpeed();
}

float Player::getFov() {
	return camera->getFov();
}

float Player::getJumpHeight() {
	return 0.0f;
}

float Player::getMouseSensitivity() {
	return 0.0f;
}

glm::vec3 Player::getPosition() {
	return camera->getPosition();
}

glm::vec2 Player::getChunkPosition() {
	return Chunk::toChunkPosition(getPosition());
}

FPSCamera* Player::getCamera() {
	return camera;
}

BoxCollision* Player::getCollisionBox() {
	return collisionBox;
}

std::vector<Block> Player::getSurroundingBlocks() {
	//std::array<std::array<std::array<Block, Chunk::CHUNK_WIDTH>, Chunk::CHUNK_HEIGHT>, Chunk::CHUNK_LENGTH>* blocks = 
		//(*world->getChunks())[std::make_pair<int, int>(getChunkPosition().x, getChunkPosition().y)].get()->getBlocks();

	std::vector<Block> surroundingBlocks;
	//surroundingBlocks.push_back(blocks[])
	return *new std::vector<Block>;
}
