// Include standard headers
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <iostream>
#include <vld.h>

// Include GLEW
#define GLEW_STATIC
#include <GL/glew.h>

// Include GLFW
#include <GLFW/glfw3.h>
GLFWwindow* window;

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>

//Include external project files
#include "Common/Util/loadShader.h"
#include "Common/Util/textureLoad.h"
#include "Common/controls.h"
#include "Common/Graphics/mesh.h"
#include "Common/Graphics/GridMesh.h"
#include "Common/Graphics/meshInstance.h"
#include "Common/Util/text2D.h"




int main(void)
{


#pragma region Configuration/Setup

	//frameTime Variables
	double lastTime = glfwGetTime();
	int nbFrames = 0;

	//Debug Frame Time String
	char frameTimeString[256] = "";

	// Initialize GLFW
	if (!glfwInit())
	{
		fprintf(stderr, "Failed to initialize GLFW\n");
		return -1;
	}

	glfwWindowHint(GLFW_SAMPLES, 1);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Open a window and create its OpenGL context
	window = glfwCreateWindow(1024, 768, "Modern OpenGL", NULL, NULL);
	if (window == NULL){
		fprintf(stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n");
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	// Initialize GLEW
	glewExperimental = true; // Needed for core profile
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		return -1;
	}

	// Config Stuff
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glPointSize(5);
	//glEnable(GL_CULL_FACE);
#pragma endregion

#pragma region Load Assets
	//Initialize Text
	initText2D("Assets/DroidSansMono.dds");
		
	// Create and compile our GLSL program from the shaders
	GLuint StandardShaderID = CreateShaderProgram("Shaders/standard.vert", "Shaders/standard.frag", NULL);
	GLuint FullbrightShaderID = CreateShaderProgram("Shaders/fullbright.vert", "Shaders/fullbright.frag", NULL);

	// Load the texture
	GLuint GridTexture = loadDDS("Assets/GridTexture.dds");
	GLuint CloudTexture = loadDDS("Assets/CloudTexture.dds");
	GLuint skySphereTexture = loadDDS("Assets/skySphere.dds");

	//Models are loaded from .obj's, changed extension to .model to avoid linker issues with VS
	//Suzanne
	Mesh * suzanne = new Mesh();
	suzanne->loadFromFile("Assets/suzanne.model");

	MeshInstance * suzanne1 = new MeshInstance(suzanne, StandardShaderID, GridTexture);
	suzanne1->setPosition(glm::vec3(0.0, 2.5, 0.0));
	
	//Sphere
	Mesh * sphere = new Mesh();
	sphere->loadFromFile("Assets/sphere.model");

	MeshInstance * skySphere = new MeshInstance(sphere, FullbrightShaderID, skySphereTexture);
	skySphere->setScale(glm::vec3(50, -50, 50));

	//Grid Mesh
	GridMesh * grid = new GridMesh(30, 30, .5, .5);
	MeshInstance * grid1 = new MeshInstance(grid, StandardShaderID, GridTexture);	

#pragma endregion
	
	do{

#pragma region frameTime Calculation

		//Updates frametime
		double currentTime = glfwGetTime();
		nbFrames++;
		if (currentTime - lastTime >= 1.0){ // If last prinf() was more than 1sec ago
			// printf and reset
			sprintf(frameTimeString, "%.2f ms/frame\n", 1000.0 / double(nbFrames));
			nbFrames = 0;
			lastTime += 1.0;
		}
#pragma endregion

		// Clear the screen
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		
#pragma region Render Here
		grid1->Render();

		suzanne1->Render();

		skySphere->Render();

		printText2D(frameTimeString, 10, 10, 26);
#pragma endregion
		
		//Swap Buffers and Poll for Input
		glfwSwapBuffers(window);
		glfwPollEvents();

	} // Check if the ESC key was pressed or the window was closed
	while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
	glfwWindowShouldClose(window) == 0);

	
	//Cleanup Shaders
	glDeleteProgram(StandardShaderID);
	glDeleteProgram(FullbrightShaderID);

	//Delete Textures
	glDeleteTextures(1, &GridTexture);
	glDeleteTextures(1, &FullbrightShaderID);
	glDeleteTextures(1, &CloudTexture);

	//Delete Loaded Meshes
	delete(suzanne);
	delete(sphere);
	delete(grid);

	//Delete MeshInstances
	delete(suzanne1);
	delete(skySphere);
	delete(grid1);

	// Delete the text's VBO, the shader and the texture
	cleanupText2D();
	

	// Close OpenGL window and terminate GLFW
	glfwTerminate();

	return 0;
}