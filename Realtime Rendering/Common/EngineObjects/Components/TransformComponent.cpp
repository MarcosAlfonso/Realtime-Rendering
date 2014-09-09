#pragma once
#include "TransformComponent.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>

TransformComponent::TransformComponent(GameEntity* parent)
{
	parentEntity = parent;
	calculateModelMatrix();
}

TransformComponent::~TransformComponent()
{

}

void TransformComponent::Cleanup()
{
	delete(this);
}


void TransformComponent::Update()
{
	//If anything has changed, update Matrix to reflect changes
	if (updateModelMatrix)
	{
		calculateModelMatrix();
	}
}

void TransformComponent::calculateModelMatrix()
{
	ModelMatrix = transMatrix * rotMatrix * scaleMatrix;
	updateModelMatrix = false;
}

void TransformComponent::setScale(glm::vec3 scale)
{
	scaleVec = scale;
	scaleMatrix = glm::mat4();
	scaleMatrix = glm::scale(scaleMatrix, scale);
	updateModelMatrix = true;
}

//Set Rotation from Eulers
void TransformComponent::setRotation(glm::vec3 eulers)
{
	rotVec = eulers;

	glm::quat tempQuat = glm::quat(eulers);
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
void TransformComponent::setPosition(glm::vec3 trans)
{
	transVec = trans;
	transMatrix = glm::mat4();
	transMatrix = glm::translate(transMatrix, trans);
	updateModelMatrix = true;
}

void TransformComponent::toString()
{
	
}

