#pragma once
#include "BaseComponent.h"

#include <glm/glm.hpp>

class TransformComponent : public BaseComponent
{
public:
	TransformComponent(BaseEntity* parent);
	~TransformComponent();

	void Update();
	void toString();
	void Cleanup();


	glm::vec3 transVec;
	glm::vec3 rotVec;
	glm::vec3 scaleVec;

	void setPosition(float x, float y, float z);
	void setRotation(float x, float y, float z);
	void setRotation(glm::quat quat);
	void setScale(float x, float y, float z);

	void calculateModelMatrix();


	glm::mat4 ModelMatrix;

private:
	bool updateModelMatrix = true;

	//Transformation 
	glm::mat4 transMatrix;
	glm::mat4 rotMatrix;
	glm::mat4 scaleMatrix;

};
