#include "BaseEntity.h"

//Base Entity, basis for all Entities
BaseEntity::BaseEntity(std::string name)
{
	Name = name;
	Transform = new TransformComponent(this);
}

BaseEntity::~BaseEntity()
{
	delete(Transform);

	for (int i = components.size()-1; i >= 0; i--)
	{
		delete(components[i]);
	}
}

//Add component to this entity
void BaseEntity::addComponent(BaseComponent* comp)
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


