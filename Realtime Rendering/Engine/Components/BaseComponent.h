#pragma once

class BaseEntity;
class BaseComponent
{
public:

	BaseEntity* parentEntity;

	virtual void Cleanup() = 0; 
	virtual void Update() = 0;
	virtual void toString() = 0;

private:

};

