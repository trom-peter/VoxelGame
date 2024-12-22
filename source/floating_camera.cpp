#include "floating_camera.h"

FloatingCamera::FloatingCamera(float fov, float width, float height, float speed, float mouseSensitivity, float nearPlane, float farPlane) : 
	FPSCamera(fov, width, height, speed, mouseSensitivity, nearPlane, farPlane) {}

void FloatingCamera::moveUp(float amount) {
	translate(up * amount);
}