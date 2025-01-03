#pragma once 

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "renderer.h"

class Camera {
public:

	Camera(float fov, float width, float height);

	glm::mat4 getViewProj();

	glm::mat4 getView();

	virtual void update();

	virtual void translate(glm::vec3 v);
	
	glm::vec3 getPosition();

	float getFov();

protected:
	glm::vec3 position;
	glm::mat4 projection;
	glm::mat4 view;
	glm::mat4 viewProj;
	float fov;
};