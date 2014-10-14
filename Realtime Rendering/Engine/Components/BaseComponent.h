#pragma once
#include <string>


//Forward Declarations
class BaseEntity;

//Component types
enum ComponentTypeEnum{ CAMERA, INPUT, PHYSICS, RENDER, TRANSFORM};


//Base Class for all Components
class BaseComponent
{
public:

	virtual ~BaseComponent(){};
	
	BaseEntity* parentEntity;
	std::string Name;

	ComponentTypeEnum componentType;

	virtual void Update() = 0;

private:

};

