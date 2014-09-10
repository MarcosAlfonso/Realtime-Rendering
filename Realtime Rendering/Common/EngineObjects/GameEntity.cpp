#include "GameEntity.h"

void GameEntity::addComponent(BaseComponent* comp)
{
	components.push_back(comp);
}
void GameEntity::Update()
{
	Transform->Update();

	for (int i = 0; i < components.size(); i++)
	{
		components[i]->Update();
	}
}
GameEntity::GameEntity(std::string name)
{
	Name = name;
	Transform = new TransformComponent(this);
}
GameEntity::~GameEntity()
{
	Transform->Cleanup();
	for (int i = 0; i < components.size(); i++)
	{
		components[i]->Cleanup();
	}
}