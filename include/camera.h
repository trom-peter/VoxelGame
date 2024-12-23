#pragma once 

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

class Camera {
public:

	Camera(float fov, float width, float height, float nearPlane, float farPlane);

	glm::mat4 getViewProj();

	glm::mat4 getView();

	virtual void update();

	virtual void translate(glm::vec3 v);
	
	glm::vec3 getPosition();

protected:
	glm::vec3 position;
	glm::mat4 projection;
	glm::mat4 view;
	glm::mat4 viewProj;

};