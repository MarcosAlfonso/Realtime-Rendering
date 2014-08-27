#pragma once
#include <vector>
#include "Components/BaseComponent.h"

class GameObject
{
public:
	GameObject();
	~GameObject();
	
	void Init();
	void Update();
	void Destroy();
	
	void addComponent(BaseComponent comp);

	glm::vec3 transVec = glm::vec3(0);
	glm::vec3 rotVec = glm::vec3(0);
	glm::vec3 scaleVec = glm::vec3(0);


private:
	std::vector<BaseComponent> components;
};

void GameObject::Init()
{
	for (int i = 0; i < components.size(); i++)
	{
		components[i].Init();
	}
}

void GameObject::Update()
{
	for (int i = 0; i < components.size(); i++)
	{
		components[i].Update();
	}
}

void GameObject::Destroy()
{
	for (int i = 0; i < components.size(); i++)
	{
		components[i].Destroy();
	}
}

void GameObject::addComponent(BaseComponent comp)
{
	components.push_back(comp);
}