// Include GLFW
#include <GLFW/glfw3.h>

extern GLFWwindow* window; // The "extern" keyword here is to access the variable "window" declared in tutorialXXX.cpp. This is a hack to keep the tutorials simple. Please avoid this.


#include <stdio.h>
#include <stdlib.h>
#include <iostream>

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;

extern char debugBuffer[];
#include "Util/DebugDisplay.h"
extern DebugDisplay * timedDebugDisplay;
extern DebugDisplay * debugDisplay;

extern float lightRot;

#include "controls.h"
#include "EngineObjects/Entities/BaseEntity.h"
#include "EngineObjects/Components/RenderComponent.h"
#include "EngineObjects/Components/PhysicsComponent.h"
#include "EngineObjects/Components/CameraComponent.h"
#include "EngineObjects/Entities/FreeCamera.h"

extern std::vector<BaseEntity*> GameEntities;
extern Mesh * sphere;
extern GLuint GridTexture;
extern GLuint StandardShaderID;

extern FreeCamera * mainCamera;
extern bool shouldClose;
extern btDiscreteDynamicsWorld* dynamicsWorld;


glm::mat4 ViewMatrix;
glm::mat4 ProjectionMatrix;

glm::mat4 getViewMatrix(){
	return ViewMatrix;
}
glm::mat4 getProjectionMatrix(){
	return ProjectionMatrix;
}


bool moveMode = true;

float moveSpeed; // 3 units / second
float mouseSpeed = 0.005f;

void KeyboardCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_Q && action == GLFW_PRESS)
	{
		timedDebugDisplay->addDebug("Spawned: Physics Sphere", 1);
		BaseEntity* physicsSphere = new BaseEntity("Physics Test Sphere");
		physicsSphere->Transform->setPosition(0, 5, 0);

		physicsSphere->addComponent(new RenderComponent(physicsSphere, sphere, StandardShaderID, GridTexture));
		physicsSphere->addComponent(new PhysicsComponent(physicsSphere, SPHERE, 1, std::vector<float>()));
		GameEntities.push_back(physicsSphere);
	}

	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		shouldClose = true;
	}

	if (key == GLFW_KEY_M && action == GLFW_PRESS)
	{
		moveMode = !moveMode;
	}
}

void MouseButtonCallback(GLFWwindow * window, int button, int action, int mods)
{
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
			
			timedDebugDisplay->addDebug(hitPhys->parentEntity->Name, 1);
		}
	}
}

void ControlInit()
{
	glfwSetKeyCallback(window, KeyboardCallback);
	glfwSetMouseButtonCallback(window, MouseButtonCallback);
}

void ControlsUpdate(){


	if (glfwGetWindowAttrib(window, GLFW_FOCUSED))
	{

		// glfwGetTime is called only once, the first time this function is called
		static double lastTime = glfwGetTime();

		// Compute time difference between current and last frame
		double currentTime = glfwGetTime();
		float deltaTime = float(currentTime - lastTime);

		// Get mouse position
		double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);

		// Reset mouse position for next frame
		glfwSetCursorPos(window, 1024 / 2, 768 / 2);


		// Compute new orientation
		mainCamera->Camera->horizontalAngle += mouseSpeed * float(1024 / 2 - xpos);
		mainCamera->Camera->verticalAngle += mouseSpeed * float(768 / 2 - ypos);

		//Prevents camera from getting flipped upside down
		mainCamera->Camera->verticalAngle = clamp(mainCamera->Camera->verticalAngle, -1.57f, 1.57f);

		moveSpeed = 5;

		if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS){
			moveSpeed = 20;
		}

		// Move forward
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS){
			mainCamera->Camera->positionOffset += mainCamera->Camera->direction * deltaTime * moveSpeed;
		}
		// Move backward
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS){
			mainCamera->Camera->positionOffset -= mainCamera->Camera->direction * deltaTime * moveSpeed;
		}
		// Strafe right
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS){
			mainCamera->Camera->positionOffset += mainCamera->Camera->right * deltaTime * moveSpeed;
		}
		// Strafe left
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS){
			mainCamera->Camera->positionOffset -= mainCamera->Camera->right * deltaTime * moveSpeed;
		}
		// Space move upward
		if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS){
			mainCamera->Camera->positionOffset += glm::vec3(0, 1, 0) * deltaTime * moveSpeed;
		}
		// Space move upward
		if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS){
			mainCamera->Camera->positionOffset += glm::vec3(0, -1, 0) * deltaTime * moveSpeed;

		}

		mainCamera->Update();

		sprintf(debugBuffer, "Cam Pos: %4.1f,%4.1f,%4.1f", mainCamera->Camera->positionOffset.x, mainCamera->Camera->positionOffset.y, mainCamera->Camera->positionOffset.z);
		debugDisplay->addDebug(debugBuffer);

		// For the next frame, the "last time" will be "now"
		lastTime = currentTime;

	}

	

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
