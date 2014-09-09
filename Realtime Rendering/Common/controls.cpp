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
#include "EngineObjects/GameEntity.h"
#include "EngineObjects/Components/RenderComponent.h"
#include "EngineObjects/Components/PhysicsComponent.h"

extern std::vector<GameEntity*> GameEntities;
extern Mesh * sphere;
extern GLuint GridTexture;
extern GLuint StandardShaderID;

extern bool shouldClose;


glm::mat4 ViewMatrix;
glm::mat4 ProjectionMatrix;

glm::mat4 getViewMatrix(){
	return ViewMatrix;
}
glm::mat4 getProjectionMatrix(){
	return ProjectionMatrix;
}


// Initial position : on +Z
glm::vec3 position = glm::vec3(2,1,8);
// Initial horizontal angle : toward -Z
float horizontalAngle = 3.14f;
// Initial vertical angle : none
float verticalAngle = 0.0f;
// Initial Field of View
float initialFov = 45.0f;

float moveSpeed; // 3 units / second
float mouseSpeed = 0.005f;

void KeyboardCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_Q && action == GLFW_PRESS)
	{
		timedDebugDisplay->addDebug("Spawned: Physics Sphere", 1);
		GameEntity* physicsSphere = new GameEntity();
		physicsSphere->Transform->setScale(glm::vec3(1, 1, 1));
		physicsSphere->Transform->setPosition(glm::vec3(0, 20, 0));

		physicsSphere->addComponent(new RenderComponent(physicsSphere, sphere, StandardShaderID, GridTexture));
		physicsSphere->addComponent(new PhysicsComponent(physicsSphere, SPHERE, 1, NULL));
		GameEntities.push_back(physicsSphere);
	}

	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		shouldClose = true;
	}
}

void ControlInit()
{
	glfwSetKeyCallback(window, KeyboardCallback);
}

void computeMatricesFromInputs(){

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
		horizontalAngle += mouseSpeed * float(1024 / 2 - xpos);
		verticalAngle += mouseSpeed * float(768 / 2 - ypos);

		//Prevents camera from getting flipped upside down
		verticalAngle = clamp(verticalAngle, -1.57f, 1.57f);

		// Direction : Spherical coordinates to Cartesian coordinates conversion
		glm::vec3 direction(
			cos(verticalAngle) * sin(horizontalAngle),
			sin(verticalAngle),
			cos(verticalAngle) * cos(horizontalAngle)
			);

		// Right vector
		glm::vec3 right = glm::vec3(
			sin(horizontalAngle - 3.14f / 2.0f),
			0,
			cos(horizontalAngle - 3.14f / 2.0f)
			);

		// Up vector
		glm::vec3 up = glm::cross(right, direction);

		moveSpeed = 5;

		if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS){
			moveSpeed = 20;
		}

		// Move forward
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS){
			position += direction * deltaTime * moveSpeed;
		}
		// Move backward
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS){
			position -= direction * deltaTime * moveSpeed;
		}
		// Strafe right
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS){
			position += right * deltaTime * moveSpeed;
		}
		// Strafe left
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS){
			position -= right * deltaTime * moveSpeed;
		}
		// Space move upward
		if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS){
			position += glm::vec3(0, 1, 0) * deltaTime * moveSpeed;
		}
		// Space move upward
		if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS){
			position += glm::vec3(0, -1, 0) * deltaTime * moveSpeed;

		}

		float FoV = initialFov;

		sprintf(debugBuffer, "Cam Pos: %4.1f,%4.1f,%4.1f", position.x, position.y, position.z );
		debugDisplay->addDebug(debugBuffer);

		// Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
		ProjectionMatrix = glm::perspective(FoV, 4.0f / 3.0f, 0.1f, 1000.0f);
		// Camera matrix
		ViewMatrix = glm::lookAt(
			position,           // Camera is here
			position + direction, // and looks here : at the same position, plus "direction"
			up                  // Head is up (set to 0,-1,0 to look upside-down)
			);

		// For the next frame, the "last time" will be "now"
		lastTime = currentTime;

	}
}

