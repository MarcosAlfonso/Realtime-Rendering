#pragma once
#include "CameraComponent.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>

//Camera Component, consisting of View and Projection Matrix
CameraComponent::CameraComponent(BaseEntity* parent)
{
	Name = "Camera Component";
	parentEntity = parent;
	componentType = CAMERA;

}

CameraComponent::~CameraComponent()
{
	delete(this);
}

//Calculates essential vectors and creates camera 
void CameraComponent::Update()
{
	//Direction vector
	direction = glm::vec3(
		cos(verticalAngle) * sin(horizontalAngle),
		sin(verticalAngle),
		cos(verticalAngle) * cos(horizontalAngle)
		);

	// Right vector
	right = glm::vec3(
		sin(horizontalAngle - 3.14f / 2.0f),
		0,
		cos(horizontalAngle - 3.14f / 2.0f)
		);

	// Up vector
	up = glm::cross(right, direction);

	// Projection matrix : 45� Field of View, 16:9 Ration, 0.1-2000.0 view range
	ProjectionMatrix = glm::perspective(fov, 16.0f / 9.0f, 0.1f, 2000.0f);
	
	// View Matrix
	ViewMatrix = glm::lookAt(
		positionOffset,           // Camera is here
		positionOffset + direction, // and looks here : at the same position, plus "direction"
		up                  // Head is up (set to 0,-1,0 to look upside-down)
		);
}
