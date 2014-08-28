#include "TransformComponent.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>

TransformComponent::TransformComponent()
{
	calculateModelMatrix();
}

TransformComponent::~TransformComponent()
{

}

void TransformComponent::Update()
{
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
	scaleMatrix = glm::scale(transMatrix, scale);
	updateModelMatrix = true;
}

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

void TransformComponent::setPosition(glm::vec3 trans)
{
	transVec = trans;
	transMatrix = glm::mat4();
	transMatrix = glm::translate(transMatrix, trans);
	updateModelMatrix = true;
}
