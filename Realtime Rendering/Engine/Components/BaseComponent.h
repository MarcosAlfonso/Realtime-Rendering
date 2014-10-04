#pragma once
#include <string>

class BaseEntity;
class BaseComponent
{
public:

	BaseEntity* parentEntity;
	std::string Name;

	virtual void Cleanup() = 0; 
	virtual void Update() = 0;

private:

};

