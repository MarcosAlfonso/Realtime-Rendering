#pragma once

class GameObject;
class BaseComponent
{
public:

	GameObject* parentObject;

	void Init(GameObject* parent)
	{
		parentObject = parent;
	};

	virtual void Update() = 0;

private:

};

