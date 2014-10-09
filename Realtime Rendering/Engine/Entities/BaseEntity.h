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
	void addComponent(std::shared_ptr<BaseComponent> comp);
	void Init();

	std::shared_ptr<TransformComponent> Transform;

	std::string Name;
	int ID;
	std::vector<std::shared_ptr<BaseComponent>> components;

	std::shared_ptr<BaseComponent> getComponent(ComponentTypeEnum _componentType);

private:
};