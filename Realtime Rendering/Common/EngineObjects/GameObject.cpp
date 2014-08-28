#include "GameObject.h"

void GameObject::addComponent(BaseComponent* comp)
{
	components.push_back(comp);
}

void GameObject::Update()
{
	Transform->Update();

	for (int i = 0; i < components.size(); i++)
	{
		components[i]->Update();
	}
}

void GameObject::Init()
{
	Transform->Init(this);

	for (int i = 0; i < components.size(); i++)
	{
		components[i]->Init(this);
	}
}

GameObject::GameObject()
{
	Transform = new TransformComponent();
}
