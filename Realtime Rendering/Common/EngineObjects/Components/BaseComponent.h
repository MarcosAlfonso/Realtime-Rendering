#pragma once

class GameEntity;
class BaseComponent
{
public:

	GameEntity* parentEntity;

	virtual void Cleanup() = 0; 
	virtual void Update() = 0;
	virtual void toString() = 0;

private:

};

