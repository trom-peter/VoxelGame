#pragma once
#include "input_handler.h"
#include "floating_camera.h"

class CameraController {
public:
	CameraController(FloatingCamera& camera);
	void handleInputs(InputHandler& ih, float delta);
private:
	FloatingCamera& camera;
};