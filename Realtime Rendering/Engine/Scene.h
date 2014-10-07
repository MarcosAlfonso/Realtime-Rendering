#pragma once
#include <vector>

class BaseEntity;

class Scene
{
public:
	Scene();
	~Scene();

	std::vector<BaseEntity*> GameEntities;
	void AddEntity(BaseEntity * ent);
};

