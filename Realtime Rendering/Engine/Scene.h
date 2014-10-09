#pragma once
#include "Entities/BaseEntity.h"


#include <vector>
#include <memory>

class BaseEntity;

class Scene
{
public:
	Scene();
	~Scene();

	std::vector<std::shared_ptr<BaseEntity>> GameEntities;
	void AddEntity(std::shared_ptr<BaseEntity> ent);
};

