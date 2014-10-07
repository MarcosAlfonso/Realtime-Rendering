#pragma once
#include "../Components/TransformComponent.h"

#include <glm/glm.hpp>

#include <vector>

//Base Entity, basis for all Entities
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
	int ID;
	std::vector<BaseComponent*> components;

	void * getComponent(ComponentTypeEnum _componentType);

private:
};