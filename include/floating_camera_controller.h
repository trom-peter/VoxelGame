#pragma once
#include "input_handler.h"
#include "floating_camera.h"

class FloatingCameraController {
public:
	FloatingCameraController(FloatingCamera& camera);
	void handleInputs(InputHandler* ih, float delta);
private:
	FloatingCamera& camera;
};