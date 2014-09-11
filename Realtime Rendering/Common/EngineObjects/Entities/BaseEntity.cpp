#include "BaseEntity.h"

void BaseEntity::addComponent(BaseComponent* comp)
{
	components.push_back(comp);
}

void BaseEntity::Update()
{
	Transform->Update();

	for (int i = 0; i < components.size(); i++)
	{
		components[i]->Update();
	}
}

BaseEntity::BaseEntity(std::string name)
{
	Name = name;
	Transform = new TransformComponent(this);
}

BaseEntity::~BaseEntity()
{
	Transform->Cleanup();
	for (int i = 0; i < components.size(); i++)
	{
		components[i]->Cleanup();
	}
}