#pragma once
#include "Components/InputComponent.h"
#include <glm/glm.hpp>

//Input Manager, handles the Input system
void addInput(InputComponent* input);
void UpdateInput();

glm::mat4 getViewMatrix();
glm::mat4 getProjectionMatrix();

void InitializeInput();

void ScreenPosToWorldRay(
	int mouseX, int mouseY,             // Mouse position, in pixels, from bottom-left corner of the window
	int screenWidth, int screenHeight,  // Window size, in pixels
	glm::mat4 ViewMatrix,               // Camera position and orientation
	glm::mat4 ProjectionMatrix,         // Camera parameters (ratio, field of view, near and far planes)
	glm::vec3& out_origin,              // Ouput : Origin of the ray. /!\ Starts at the near plane, so if you want the ray to start at the camera's position instead, ignore this.
	glm::vec3& out_direction            // Ouput : Direction, in world space, of the ray that goes "through" the mouse.
	);
