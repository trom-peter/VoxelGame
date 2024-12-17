#include "floating_camera.h"

FloatingCamera::FloatingCamera(float fov, float width, float height, float speed, float mouseSensitivity) : 
	FPSCamera(fov, width, height, speed, mouseSensitivity) {}

void FloatingCamera::moveUp(float amount) {
	translate(up * amount);
}