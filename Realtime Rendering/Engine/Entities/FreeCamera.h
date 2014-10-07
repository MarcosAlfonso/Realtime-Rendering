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

//Free Camera Input Implentation of InputComponent interface 
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
	 
	void Update()
	{
		//Camera movement speed
		moveSpeed = 6;

		//Sprint camera
		if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS){
			moveSpeed = 16;
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

		//Right clicking rotates camera
		if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)	{

			//Hides cursor, and makes mouse boundless for infinite rotation
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

			//Moves mouse pointer to middle of screen on start
			if (mouseControlRefresh)
				glfwSetCursorPos(window, screenX / 2, screenY / 2);
			
			// Get mouse position
			double xpos, ypos;
			glfwGetCursorPos(window, &xpos, &ypos);

			// Reset mouse position for next frame
			glfwSetCursorPos(window, screenX / 2, screenY / 2);
			
			// Compute new orientation by seeing how much mouse moved
			Camera->horizontalAngle += mouseSpeed * float(screenX / 2 - xpos);
			Camera->verticalAngle += mouseSpeed * float(screenY / 2 - ypos);

			//Prevents camera from getting flipped upside down
			Camera->verticalAngle = glm::clamp(Camera->verticalAngle, -1.57f, 1.57f);

			//After its happened the first time we don't need to pre set the mouse to middle
			mouseControlRefresh = false;
		}
		else
		{
			// Get mouse position, saves it for later
			glfwGetCursorPos(window, &oldMouseX, &oldMouseY);
			mouseControlRefresh = true;

		}

	}

	void KeyboardInputCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
	{

	}

	void MouseInputCallback(GLFWwindow* window, int button, int action, int mods)
	{
		//If right click is released
		if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_RELEASE)
		{
			//Make mouse visible and normal, and move it back to where it was when you first clicked
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
			glfwSetCursorPos(window, oldMouseX, oldMouseY);

		}

	}

};

//Prefab: Free Camera
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
		delete(Camera);
	}

private:




};
