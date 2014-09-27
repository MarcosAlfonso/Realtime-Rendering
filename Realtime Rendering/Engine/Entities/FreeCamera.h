#pragma once
#include "BaseEntity.h"

#include "../InputManager.h"

#include "../Components/TransformComponent.h"
#include "../Components/CameraComponent.h"
#include "../Components/InputComponent.h"

#include <glm/glm.hpp>

#define GLEW_STATIC
#include <gl/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <vector>

extern float DeltaTime;

extern int screenX;
extern int screenY;

extern GLFWwindow* window;

class FreeCameraInput : public InputComponent
{


public:

	CameraComponent* Camera;
	bool mouseControlRefresh = false;
	float moveSpeed = 3; // 3 units / second
	float mouseSpeed = 0.005f;

	double oldMouseX, oldMouseY;

	FreeCameraInput(BaseEntity* parent, CameraComponent* _camera)
	{
		parentEntity = parent;
		Camera = _camera;

	}
	
	~FreeCameraInput()
	{
		delete(this);
	}

	void Cleanup()
	{
		delete(this);
	}

	void Update()
	{
		moveSpeed = 6;
		if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS){
			moveSpeed = 12;
		}

		// Move forward
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS){
			Camera->positionOffset += Camera->direction * DeltaTime * moveSpeed;
		}
		// Move backward
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS){
			Camera->positionOffset -= Camera->direction * DeltaTime * moveSpeed;
		}
		// Strafe right
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS){
			Camera->positionOffset += Camera->right * DeltaTime * moveSpeed;
		}
		// Strafe left
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS){
			Camera->positionOffset -= Camera->right * DeltaTime * moveSpeed;
		}
		// Space move upward
		if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS){
			Camera->positionOffset += glm::vec3(0, 1, 0) * DeltaTime * moveSpeed;
		}
		// Space move upward
		if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS){
			Camera->positionOffset += glm::vec3(0, -1, 0) * DeltaTime * moveSpeed;
		}

 
		if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)	{

			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

			if (mouseControlRefresh)
				glfwSetCursorPos(window, screenX / 2, screenY / 2);
			
			// Get mouse position
			double xpos, ypos;
			glfwGetCursorPos(window, &xpos, &ypos);

			// Reset mouse position for next frame
			glfwSetCursorPos(window, screenX / 2, screenY / 2);
			
			// Compute new orientation
			Camera->horizontalAngle += mouseSpeed * float(screenX / 2 - xpos);
			Camera->verticalAngle += mouseSpeed * float(screenY / 2 - ypos);

			//Prevents camera from getting flipped upside down
			Camera->verticalAngle = glm::clamp(Camera->verticalAngle, -1.57f, 1.57f);

			mouseControlRefresh = false;
		}
		else
		{
			// Get mouse position
			glfwGetCursorPos(window, &oldMouseX, &oldMouseY);
			mouseControlRefresh = true;

		}

	}

	void KeyboardInputCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
	{

	}

	void MouseInputCallback(GLFWwindow* window, int button, int action, int mods)
	{
		if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_RELEASE)
		{
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
			glfwSetCursorPos(window, oldMouseX, oldMouseY);

		}

	}

};

class FreeCamera : public BaseEntity
{
public:	

	CameraComponent * Camera;
	InputComponent * Input;

	FreeCamera(std::string name) : BaseEntity(name)
	{
		Camera = new CameraComponent(this);
		Input = new FreeCameraInput(this, Camera);

		addInput(Input);

		addComponent(Camera);
	}

	~FreeCamera()
	{
		Camera->Cleanup();
	}

private:




};
