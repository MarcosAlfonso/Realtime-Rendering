#pragma once

class GameEntity;
class BaseComponent
{
public:

	GameEntity* parentEntity;

	virtual void Update() = 0;
	virtual void toString() = 0;

private:

};

