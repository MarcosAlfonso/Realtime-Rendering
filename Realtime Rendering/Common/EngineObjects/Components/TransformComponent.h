#pragma once
#include <glm/glm.hpp>
#include "BaseComponent.h"

class TransformComponent : public BaseComponent
{
public:
	TransformComponent(GameEntity* parent);
	~TransformComponent();

	void Update();
	void toString();
	void Cleanup();


	glm::vec3 transVec = glm::vec3(0);
	glm::vec3 rotVec = glm::vec3(0);
	glm::vec3 scaleVec = glm::vec3(0);

	void setPosition(glm::vec3 trans);
	void setRotation(glm::vec3 eulers);
	void setRotation(glm::quat quat);
	void setScale(glm::vec3 scale);

	void calculateModelMatrix();


	glm::mat4 ModelMatrix;


private:
	bool updateModelMatrix;

	//Transformation 
	glm::mat4 transMatrix;
	glm::mat4 rotMatrix;
	glm::mat4 scaleMatrix;

};
