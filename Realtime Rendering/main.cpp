// Include standard headers
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <iostream>
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

#include "Common/loadShader.hpp"
#include "Common/textureLoad.hpp"
#include "Common/controls.hpp"
#include "Common/mesh.hpp"
#include "Common/meshInstance.hpp"
#include "Common/text2D.hpp"

int main(void)
{
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
	glClearColor(0.0f, 0.05f, 0.0f, 0.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glEnable(GL_CULL_FACE);

	//Initialize Text
	initText2D("Assets/DroidSansMono.dds");
		
	// Create and compile our GLSL program from the shaders
	GLuint StandardShaderID = LoadShaders("Shaders/standard.vertex", "Shaders/standard.fragment");
	GLuint FullbrightShaderID = LoadShaders("Shaders/fullbright.vertex", "Shaders/fullbright.fragment");

	// Load the texture
	GLuint GridTexture = loadDDS("Assets/GridTexture.dds");
	GLuint CloudTexture = loadDDS("Assets/CloudTexture.dds");

	// Get a handle for our "LightPosition" uniform
	GLuint LightID = glGetUniformLocation(StandardShaderID, "LightPosition_worldspace");

	// For speed computation
	double lastTime = glfwGetTime();
	int nbFrames = 0;

	Mesh * torus = new Mesh("Assets/torusTri.obj");
	Mesh * cube = new Mesh("Assets/cube.obj");
	Mesh * suzanne = new Mesh("Assets/suzanne.obj");

	MeshInstance * cube1 = new MeshInstance(cube, StandardShaderID, GridTexture);
	MeshInstance * cube2 = new MeshInstance(cube, FullbrightShaderID, CloudTexture);
	cube1->setPosition(glm::vec3(0.0, 0.0, 0.0));
	cube2->setPosition(glm::vec3(3.0, 0.0, 0.0));

	MeshInstance * suzanne1 = new MeshInstance(suzanne, StandardShaderID, GridTexture);
	MeshInstance * suzanne2 = new MeshInstance(suzanne, FullbrightShaderID, CloudTexture);
	suzanne1->setPosition(glm::vec3(0.0, 2.5, 0.0));
	suzanne2->setPosition(glm::vec3(3.0, 2.5, 0.0));

	do{

		// Measure speed
		double currentTime = glfwGetTime();
		nbFrames++;
		if (currentTime - lastTime >= 1.0){ // If last prinf() was more than 1sec ago
			// printf and reset
			sprintf(frameTimeString, "%.2f ms/frame\n", 1000.0 / double(nbFrames));
			nbFrames = 0;
			lastTime += 1.0;
		}

		// Clear the screen
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Use our shader
		glUseProgram(StandardShaderID);

		glm::vec3 lightPos = glm::vec3(0, 0, 4);
		glUniform3f(LightID, lightPos.x, lightPos.y, lightPos.z);

		cube1->Render();
		cube2->Render();

		suzanne1->Render();
		suzanne2->Render();
		
		//Draw text
		printText2D(frameTimeString, 10, 10, 26);

		// Swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();

	} // Check if the ESC key was pressed or the window was closed
	while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
	glfwWindowShouldClose(window) == 0);

	// Cleanup VBO and shader
	glDeleteProgram(StandardShaderID);
	glDeleteTextures(1, &GridTexture);

	// Delete the text's VBO, the shader and the texture
	cleanupText2D();

	// Close OpenGL window and terminate GLFW
	glfwTerminate();

	return 0;
}