#pragma once
#include "BaseComponent.h"

#include <glm/glm.hpp>

class CameraComponent : public BaseComponent
{
public:
	CameraComponent(BaseEntity* parent);
	~CameraComponent();

	void Update();
	void Cleanup();
	
	glm::mat4 ViewMatrix;
	glm::mat4 ProjectionMatrix;

	glm::vec3 positionOffset = glm::vec3(0, 0, 0);
	float fov = 45.0f;
	float horizontalAngle = 0.0f;
	float verticalAngle = 0.0f;


	glm::vec3 direction;
	glm::vec3 right;
	glm::vec3 up;

private:

};
