#include "BaseEntity.h"

//Base Entity, basis for all Entities
BaseEntity::BaseEntity(std::string name)
{
	Name = name;
	Transform = std::shared_ptr<TransformComponent>(new TransformComponent(std::shared_ptr<BaseEntity>(this)));
}

BaseEntity::~BaseEntity()
{

}

//Add component to this entity
void BaseEntity::addComponent(std::shared_ptr<BaseComponent> comp)
{
	components.push_back(comp);
}

//Iterates through all components and calls the update
void BaseEntity::Update()
{
	Transform->Update();

	for (int i = 0; i < components.size(); i++)
	{
		components[i]->Update();
	}
}

std::shared_ptr<BaseComponent> BaseEntity::getComponent(ComponentTypeEnum _componentType)
{

	for (int i = 0; i < components.size(); i++)
	{
		if (components[i]->componentType == _componentType)
		{
			return components[i];
		}
	}

	return NULL;
}
