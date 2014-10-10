#pragma once
#include "BaseComponent.h"

#include <glm/glm.hpp>

//Camera Component, consisting of View and Projection Matrix
class CameraComponent : public BaseComponent
{
public:
	CameraComponent(BaseEntity * parent);
	~CameraComponent();

	void Update();
	
	glm::mat4 ViewMatrix;
	glm::mat4 ProjectionMatrix;

	//Offset from transform
	glm::vec3 positionOffset = glm::vec3(0, 0, 0);

	float fov = 45.0f;
	
	//Rotation angles
	float horizontalAngle = 0.0f;
	float verticalAngle = 0.0f;


	glm::vec3 direction;
	glm::vec3 right;
	glm::vec3 up;

private:

};
