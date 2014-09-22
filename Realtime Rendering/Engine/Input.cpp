#pragma once
#include "Input.h"

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


glm::mat4 ViewMatrix;
glm::mat4 ProjectionMatrix;

glm::mat4 getViewMatrix(){
	return ViewMatrix;
}
glm::mat4 getProjectionMatrix(){
	return ProjectionMatrix;
}

std::vector<InputComponent*> InputList;

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
			getViewMatrix(),               // Camera position and orientation
			getProjectionMatrix(),         // Camera parameters (ratio, field of view, near and far planes)
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
	CEGUI::MouseButton CEGUIMouseButton;

	//Convert from GLFW mouse to CEGUI mouse
	switch (button)
	{
	case GLFW_MOUSE_BUTTON_LEFT:
		CEGUIMouseButton = CEGUI::MouseButton::LeftButton;
	case GLFW_MOUSE_BUTTON_RIGHT:
		CEGUIMouseButton = CEGUI::MouseButton::RightButton;
	}

	//Inject Up or Down based on GLFW action
	if (action == GLFW_PRESS)
	{
		CEGUI::System::getSingleton().getDefaultGUIContext().injectMouseButtonDown(CEGUIMouseButton);
	}
	else if (action == GLFW_RELEASE)
	{
		CEGUI::System::getSingleton().getDefaultGUIContext().injectMouseButtonDown(CEGUIMouseButton);
	}

}

void ControlInit()
{
	glfwSetKeyCallback(window, KeyboardCallback);
	glfwSetMouseButtonCallback(window, MouseButtonCallback);
}

void ControlsUpdate(){

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
	CEGUI::System::getSingleton().getDefaultGUIContext().injectMousePosition(xpos, ypos);

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
