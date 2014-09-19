#pragma once
#include "BaseEntity.h"

#include "../Input.h"

#include "../Components/TransformComponent.h"
#include "../Components/CameraComponent.h"
#include "../Components/InputComponent.h"

#include <glm/glm.hpp>

#include <vector>

class FreeCamera : public BaseEntity
{
public:	

	CameraComponent * Camera;
	InputComponent * Input;

	void (*keyboardFunc)(GLFWwindow* window, int key, int scancode, int action, int mods)
	{

	};

	void (*mouseFunc)(GLFWwindow* window, int button, int action, int mods)
	{

	};

	FreeCamera(std::string name) : BaseEntity(name)
	{
		Camera = new CameraComponent(this);
		Input = new InputComponent(this);

		Input->KeyboardInputFunc = keyboardFunc;
		Input->MouseInputFunc = mouseFunc;

		addInput(Input);

		addComponent(Camera);
	}

	~FreeCamera()
	{
		Camera->Cleanup();
	}

private:




};