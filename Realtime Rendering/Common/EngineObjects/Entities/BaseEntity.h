#pragma once
#include <vector>
#include <glm/glm.hpp>
#include "../Components/TransformComponent.h"
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


private:
	std::vector<BaseComponent*> components;
};