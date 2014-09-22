#pragma once
#include "CameraComponent.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>

CameraComponent::CameraComponent(BaseEntity* parent)
{
	parentEntity = parent;

}

CameraComponent::~CameraComponent()
{

}

void CameraComponent::Cleanup()
{
	delete(this);
}


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

	// Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
	ProjectionMatrix = glm::perspective(fov, 4.0f / 3.0f, 0.1f, 1000.0f);
	
	// Camera matrix
	ViewMatrix = glm::lookAt(
		positionOffset,           // Camera is here
		positionOffset + direction, // and looks here : at the same position, plus "direction"
		up                  // Head is up (set to 0,-1,0 to look upside-down)
		);
}
