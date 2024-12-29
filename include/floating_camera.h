#pragma once

#include "fps_camera.h"

class FloatingCamera : public FPSCamera {

public:
	FloatingCamera(float fov, float width, float height, float speed, float mouseSensitivity);
	void moveUp(float amount);

};