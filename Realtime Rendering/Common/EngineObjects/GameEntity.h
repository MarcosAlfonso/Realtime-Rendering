#pragma once
#include <vector>
#include <glm/glm.hpp>
#include "Components/TransformComponent.h"
class GameEntity
{
public:
	GameEntity();;
	~GameEntity();
	void Update();
	void addComponent(BaseComponent* comp);
	void Init();
	TransformComponent* Transform;
	int ID;
private:
	std::vector<BaseComponent*> components;
};