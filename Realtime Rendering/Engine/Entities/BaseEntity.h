#pragma once
#include "../Components/TransformComponent.h"

#include <glm/glm.hpp>

#include <vector>

class BaseEntity
{
public:
	BaseEntity(std::string name);
	~BaseEntity();

	void Update();
	void addComponent(BaseComponent* comp);
	void Init();

	TransformComponent* Transform;

	std::string Name;
	std::vector<BaseComponent*> components;



private:
};