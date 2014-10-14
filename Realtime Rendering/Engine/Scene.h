#pragma once
#include "Entities/BaseEntity.h"


#include <vector>


class BaseEntity;

class Scene
{
public:
	Scene();
	~Scene();


	std::vector<BaseEntity*> GameEntities;
	void AddEntity(BaseEntity * ent);
	void LoadScene();
};

