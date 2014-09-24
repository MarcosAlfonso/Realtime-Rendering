#pragma once
#include "InputManager.h"

#include "Entities/BaseEntity.h"
#include "Components/RenderComponent.h"
#include "Components/PhysicsComponent.h"
#include "Components/CameraComponent.h"
#include "Entities/FreeCamera.h"

#define GLEW_STATIC
#include <gl/glew.h>
#include <GLFW/glfw3.h>

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

//Bullet Physics
#include <btBulletCollisionCommon.h>
#include <btBulletDynamicsCommon.h>

// Include GLFW
extern GLFWwindow* window; 

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <CEGUI/GUILayout_xmlHandler.h>

extern std::vector<BaseEntity*> GameEntities;

extern Mesh * sphere;
extern GLuint GridTexture;
extern GLuint StandardShaderID;
extern float lightRot;

extern FreeCamera * mainCamera;
extern btDiscreteDynamicsWorld* dynamicsWorld;

std::vector<InputComponent*> InputList;

CEGUI::MouseButton GlfwToCeguiButton(int glfwButton);
unsigned int GlfwToCeguiKey(int glfwKey);


void addInput(InputComponent* input)
{
	InputList.push_back(input);
}

void KeyboardCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	//Global Keyboard Input
	if (key == GLFW_KEY_Q && action == GLFW_PRESS)
	{
		BaseEntity* physicsSphere = new BaseEntity("Physics Test Sphere");
		physicsSphere->Transform->setPosition(0, 5, 0);

		physicsSphere->addComponent(new RenderComponent(physicsSphere, sphere, StandardShaderID, GridTexture));
		physicsSphere->addComponent(new PhysicsComponent(physicsSphere, SPHERE, 1, std::vector<float>()));
		GameEntities.push_back(physicsSphere);
	}

	//InputComponent Keyboards Callbacks
	for (int i = 0; i < InputList.size(); i++)
	{
		InputList[i]->KeyboardInputCallback(window, key, scancode, action, mods);
	}

	//CEGUI Keyboard Callbacks 
	if (action == GLFW_PRESS)
	{
		//unsigned int 
		CEGUI::System::getSingleton().getDefaultGUIContext().injectChar(key);
	}
}

void MouseButtonCallback(GLFWwindow * window, int button, int action, int mods)
{
	//Global Mouse Button Input
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
	{
		double * mouseX = new double;
		double * mouseY = new double;

		glfwGetCursorPos(window, mouseX, mouseY);

		glm::vec3 out_origin;
		glm::vec3 out_direction;

		
		ScreenPosToWorldRay(
			(int)*mouseX, (int)*mouseY,             // Mouse position, in pixels, from bottom-left corner of the window
			1024, 768,  // Window size, in pixels
			mainCamera->Camera->ViewMatrix,               // Camera position and orientation
			mainCamera->Camera->ProjectionMatrix,         // Camera parameters (ratio, field of view, near and far planes)
			out_origin,              // Ouput : Origin of the ray. /!\ Starts at the near plane, so if you want the ray to start at the camera's position instead, ignore this.
			out_direction            // Ouput : Direction, in world space, of the ray that goes "through" the mouse.
			);

		out_direction = out_direction*1000.0f;

		btCollisionWorld::ClosestRayResultCallback RayCallback(
			btVector3(out_origin.x, out_origin.y, out_origin.z),
			btVector3(out_direction.x, out_direction.y, out_direction.z)
			);
		dynamicsWorld->rayTest(
			btVector3(out_origin.x, out_origin.y, out_origin.z),
			btVector3(out_direction.x, out_direction.y, out_direction.z),
			RayCallback
			);

		if (RayCallback.hasHit()) {
			void * hitPointer = RayCallback.m_collisionObject->getUserPointer();
			PhysicsComponent* hitPhys = static_cast<PhysicsComponent*>(hitPointer);
			
		}
	}

	//InputComponent Mouse Button Callback
	for (int i = 0; i < InputList.size(); i++)
	{
		InputList[i]->MouseInputCallback(window, button, action, mods);
	}

	//CEGUI Mouse Button Injections
	CEGUI::MouseButton CEGUIMouseButton = GlfwToCeguiButton(button);

	//Inject Up or Down based on GLFW action
	if (action == GLFW_PRESS)
	{
		bool test =  CEGUI::System::getSingleton().getDefaultGUIContext().injectMouseButtonDown(CEGUIMouseButton);
		return;
	}
	else if (action == GLFW_RELEASE)
	{
		CEGUI::System::getSingleton().getDefaultGUIContext().injectMouseButtonUp(CEGUIMouseButton);
	}

}

void InitializeInput()
{
	glfwSetKeyCallback(window, KeyboardCallback);
	glfwSetMouseButtonCallback(window, MouseButtonCallback);
}

void UpdateInput(){

	//Update Input Components
	if (glfwGetWindowAttrib(window, GLFW_FOCUSED))
	{
		for (int i = 0; i < InputList.size(); i++)
		{
			InputList[i]->Update();
		}
	}	

	//CEGUI Mouse Position Injection 
	double xpos, ypos;
	glfwGetCursorPos(window, &xpos, &ypos);
	bool test =	CEGUI::System::getSingleton().getDefaultGUIContext().injectMousePosition(xpos, ypos);
	return;
}

void ScreenPosToWorldRay(
	int mouseX, int mouseY,             // Mouse position, in pixels, from bottom-left corner of the window
	int screenWidth, int screenHeight,  // Window size, in pixels
	glm::mat4 ViewMatrix,               // Camera position and orientation
	glm::mat4 ProjectionMatrix,         // Camera parameters (ratio, field of view, near and far planes)
	glm::vec3& out_origin,              // Ouput : Origin of the ray. /!\ Starts at the near plane, so if you want the ray to start at the camera's position instead, ignore this.
	glm::vec3& out_direction            // Ouput : Direction, in world space, of the ray that goes "through" the mouse.
	){

	// The ray Start and End positions, in Normalized Device Coordinates (Have you read Tutorial 4 ?)
	glm::vec4 lRayStart_NDC(
		((float)mouseX / (float)screenWidth - 0.5f) * 2.0f, // [0,1024] -> [-1,1]
		((float)mouseY / (float)screenHeight - 0.5f) * 2.0f, // [0, 768] -> [-1,1]
		-1.0, // The near plane maps to Z=-1 in Normalized Device Coordinates
		1.0f
		);
	glm::vec4 lRayEnd_NDC(
		((float)mouseX / (float)screenWidth - 0.5f) * 2.0f,
		((float)mouseY / (float)screenHeight - 0.5f) * 2.0f,
		0.0,
		1.0f
		);


	// The Projection matrix goes from Camera Space to NDC.
	// So inverse(ProjectionMatrix) goes from NDC to Camera Space.
	glm::mat4 InverseProjectionMatrix = glm::inverse(ProjectionMatrix);

	// The View Matrix goes from World Space to Camera Space.
	// So inverse(ViewMatrix) goes from Camera Space to World Space.
	glm::mat4 InverseViewMatrix = glm::inverse(ViewMatrix);

	glm::vec4 lRayStart_camera = InverseProjectionMatrix * lRayStart_NDC;    lRayStart_camera /= lRayStart_camera.w;
	glm::vec4 lRayStart_world = InverseViewMatrix       * lRayStart_camera; lRayStart_world /= lRayStart_world.w;
	glm::vec4 lRayEnd_camera = InverseProjectionMatrix * lRayEnd_NDC;      lRayEnd_camera /= lRayEnd_camera.w;
	glm::vec4 lRayEnd_world = InverseViewMatrix       * lRayEnd_camera;   lRayEnd_world /= lRayEnd_world.w;


	// Faster way (just one inverse)
	//glm::mat4 M = glm::inverse(ProjectionMatrix * ViewMatrix);
	//glm::vec4 lRayStart_world = M * lRayStart_NDC; lRayStart_world/=lRayStart_world.w;
	//glm::vec4 lRayEnd_world   = M * lRayEnd_NDC  ; lRayEnd_world  /=lRayEnd_world.w;


	glm::vec3 lRayDir_world(lRayEnd_world - lRayStart_world);
	lRayDir_world = glm::normalize(lRayDir_world);


	out_origin = glm::vec3(lRayStart_world);
	out_direction = glm::normalize(lRayDir_world);
}

CEGUI::MouseButton GlfwToCeguiButton(int glfwButton)
{
	switch (glfwButton)
	{
	case GLFW_MOUSE_BUTTON_LEFT: return CEGUI::LeftButton;
	case GLFW_MOUSE_BUTTON_RIGHT: return CEGUI::RightButton;
	case GLFW_MOUSE_BUTTON_MIDDLE: return CEGUI::MiddleButton;
	default: return CEGUI::NoButton;
	}
}

unsigned int GlfwToCeguiKey(int glfwKey)
{
	switch (glfwKey)
	{
	case GLFW_KEY_ESCAPE: return CEGUI::Key::Escape;
	case GLFW_KEY_F1: return CEGUI::Key::F1;
	case GLFW_KEY_F2: return CEGUI::Key::F2;
	case GLFW_KEY_F3: return CEGUI::Key::F3;
	case GLFW_KEY_F4: return CEGUI::Key::F4;
	case GLFW_KEY_F5: return CEGUI::Key::F5;
	case GLFW_KEY_F6: return CEGUI::Key::F6;
	case GLFW_KEY_F7: return CEGUI::Key::F7;
	case GLFW_KEY_F8: return CEGUI::Key::F8;
	case GLFW_KEY_F9: return CEGUI::Key::F9;
	case GLFW_KEY_F10: return CEGUI::Key::F10;
	case GLFW_KEY_F11: return CEGUI::Key::F11;
	case GLFW_KEY_F12: return CEGUI::Key::F12;
	case GLFW_KEY_F13: return CEGUI::Key::F13;
	case GLFW_KEY_F14: return CEGUI::Key::F14;
	case GLFW_KEY_F15: return CEGUI::Key::F15;
	case GLFW_KEY_UP: return CEGUI::Key::ArrowUp;
	case GLFW_KEY_DOWN: return CEGUI::Key::ArrowDown;
	case GLFW_KEY_LEFT: return CEGUI::Key::ArrowLeft;
	case GLFW_KEY_RIGHT: return CEGUI::Key::ArrowRight;
	case GLFW_KEY_LEFT_SHIFT: return CEGUI::Key::LeftShift;
	case GLFW_KEY_RIGHT_SHIFT: return CEGUI::Key::RightShift;
	case GLFW_KEY_LEFT_CONTROL: return CEGUI::Key::LeftControl;
	case GLFW_KEY_RIGHT_CONTROL: return CEGUI::Key::RightControl;
	case GLFW_KEY_LEFT_ALT: return CEGUI::Key::LeftAlt;
	case GLFW_KEY_RIGHT_ALT: return CEGUI::Key::RightAlt;
	case GLFW_KEY_TAB: return CEGUI::Key::Tab;
	case GLFW_KEY_ENTER: return CEGUI::Key::Return;
	case GLFW_KEY_BACKSPACE: return CEGUI::Key::Backspace;
	case GLFW_KEY_INSERT: return CEGUI::Key::Insert;
	case GLFW_KEY_DELETE: return CEGUI::Key::Delete;
	case GLFW_KEY_PAGE_UP: return CEGUI::Key::PageUp;
	case GLFW_KEY_PAGE_DOWN: return CEGUI::Key::PageDown;
	case GLFW_KEY_HOME: return CEGUI::Key::Home;
	case GLFW_KEY_END: return CEGUI::Key::End;
	case GLFW_KEY_KP_ENTER: return CEGUI::Key::NumpadEnter;
	default: return 0;
	}
}