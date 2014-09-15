#pragma once
#include <vector>
#include <glm/glm.hpp>
#include "../Components/TransformComponent.h"
#include "BaseEntity.h"
#include "../Components/CameraComponent.h"

class FreeCamera : public BaseEntity
{
public:	

	CameraComponent * Camera;

	FreeCamera(std::string name) : BaseEntity(name)
	{
		Camera = new CameraComponent(this);
		Transform = new TransformComponent(this);
		addComponent(Camera);
	}

	~FreeCamera()
	{
		Camera->Cleanup();
	}


};