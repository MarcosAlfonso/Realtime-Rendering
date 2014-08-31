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

GameEntity::GameEntity()
{
	Transform = new TransformComponent(this);
}
