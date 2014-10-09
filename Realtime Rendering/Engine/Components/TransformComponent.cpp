#pragma once
#include "TransformComponent.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>

//Transform Component, required component for all entities
TransformComponent::TransformComponent(std::shared_ptr<BaseEntity> parent)
{
	Name = "Transform Component";
	parentEntity = parent;
	componentType = TRANSFORM;

	transVec = glm::vec3(0);
	rotVec = glm::vec3(0);
	scaleVec = glm::vec3(1, 1, 1);

	calculateModelMatrix();
}

TransformComponent::~TransformComponent()
{

}

void TransformComponent::Update()
{
	//If anything has changed, update Matrix to reflect changes
	if (updateModelMatrix)
	{
		calculateModelMatrix();
	}
}

//Matrix multiplication to figure out model matrix
void TransformComponent::calculateModelMatrix()
{
	ModelMatrix = transMatrix * rotMatrix * scaleMatrix;
	updateModelMatrix = false;
}

//Set non uniform scale
void TransformComponent::setScale(float x, float y, float z)
{
	scaleVec = glm::vec3(x, y, z);
	scaleMatrix = glm::mat4();
	scaleMatrix = glm::scale(scaleMatrix, scaleVec);
	updateModelMatrix = true;
}

//Set Rotation from Eulers
void TransformComponent::setRotation(float x, float y, float z)
{
	rotVec = glm::vec3(x, y, z);

	glm::quat tempQuat = glm::quat(rotVec);
	float angle = glm::angle(tempQuat);
	glm::vec3 axis = glm::axis(tempQuat);

	rotMatrix = glm::mat4();
	rotMatrix = glm::rotate(transMatrix, angle, axis);
	updateModelMatrix = true;
}

//Set rotation from Quaternion
void TransformComponent::setRotation(glm::quat quat)
{
	float angle = glm::angle(quat);
	glm::vec3 axis = glm::axis(quat);

	rotMatrix = glm::mat4();
	rotMatrix = glm::rotate(rotMatrix, angle, axis);
	updateModelMatrix = true;
}

//Set position
void TransformComponent::setPosition(float x, float y, float z)
{
	transVec = glm::vec3(x,y,z);
	transMatrix = glm::mat4();
	transMatrix = glm::translate(transMatrix, transVec);
	updateModelMatrix = true;
}