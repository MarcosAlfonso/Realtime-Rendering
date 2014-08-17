// Include standard headers
#include <vector>
#include <iostream>

//Uncomment for leak detection
//#include <vld.h>

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
#include "Common/Util/DebugDisplay.h"

#pragma region Declarations
void SetupConfiguration();
void LoadAssets();
void Render();
void CleanupMemory();
void CalculateFrameTime();

//Debug String
char debugBuffer[512] = "";
char frameTimeBuffer[128] = "";
int vertexCount;

double lastTime;
int nbFrames = 0;

// Create and compile our GLSL program from the shaders
GLuint StandardShaderID;
GLuint FullbrightShaderID;

// Load the texture
GLuint GridTexture;
GLuint CloudTexture;
GLuint skySphereTexture;

//Models are loaded from .obj's, changed extension to .model to avoid linker issues with VS
//Suzanne
Mesh * suzanne;
MeshInstance * suzanne1;

//Sphere
Mesh * sphere;
MeshInstance * skySphere;

//Grid Mesh
GridMesh * grid;
MeshInstance * grid1;

DebugDisplay * debugDisplay;
DebugDisplay * timedDebugDisplay;
#pragma endregion 

int main(void)
{
	debugDisplay = new DebugDisplay(glm::vec2(10, 565), false);
	timedDebugDisplay = new DebugDisplay(glm::vec2(550, 565), true);

	SetupConfiguration();

	LoadAssets();
	
	do{

		CalculateFrameTime();
		
		Render();

		glfwPollEvents();

	} // Check if the ESC key was pressed or the window was closed
	while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
	glfwWindowShouldClose(window) == 0);

	CleanupMemory();

	glfwTerminate();

	return 0;
}

void SetupConfiguration()
{
	//frameTime Update
	lastTime = glfwGetTime();

	// Initialize GLFW
	if (!glfwInit())
	{
		fprintf(stderr, "Failed to initialize GLFW\n");
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
	}
	glfwMakeContextCurrent(window);

	// Initialize GLEW
	glewExperimental = true; // Needed for core profile
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
	}

	// Config Stuff
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glPointSize(5);
	//glEnable(GL_CULL_FACE);
}

void LoadAssets()
{
	//Initialize Text
	initText2D("Assets/DroidSansMono.dds");

	// Create and compile our GLSL program from the shaders
	StandardShaderID = CreateShaderProgram("Shaders/standard.vert", "Shaders/standard.frag", NULL);
	FullbrightShaderID = CreateShaderProgram("Shaders/fullbright.vert", "Shaders/fullbright.frag", NULL);

	// Load the texture
	GridTexture = loadDDS("Assets/GridTexture.dds");
	CloudTexture = loadDDS("Assets/CloudTexture.dds");
	skySphereTexture = loadDDS("Assets/skySphere.dds");

	//Models are loaded from .obj's, changed extension to .model to avoid linker issues with VS
	//Suzanne
	suzanne = new Mesh();
	suzanne->loadFromFile("Assets/suzanne.model");

	suzanne1 = new MeshInstance(suzanne, StandardShaderID, GridTexture);
	suzanne1->setPosition(glm::vec3(0.0, 2.5, 0.0));

	//Sphere
	sphere = new Mesh();
	sphere->loadFromFile("Assets/sphere.model");

	skySphere = new MeshInstance(sphere, FullbrightShaderID, skySphereTexture);
	skySphere->setScale(glm::vec3(50, -50, 50));

	//Grid Mesh
	grid = new GridMesh(100, 100, 1, 1);
	grid1 = new MeshInstance(grid, StandardShaderID, GridTexture);
}

void Render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	computeMatricesFromInputs();

	vertexCount = 0;

	grid1->Render();

	suzanne1->Render();

	skySphere->Render();

	sprintf(debugBuffer, "Vertex Count: %d", vertexCount);
	debugDisplay->addDebug(debugBuffer);

	timedDebugDisplay->Draw();
	debugDisplay->Draw();
	
	glfwSwapBuffers(window);
}

void CleanupMemory()
{
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
}

void CalculateFrameTime()
{
	//Updates frametime
	double currentTime = glfwGetTime();
	nbFrames++;
	if (currentTime - lastTime >= 1.0){ // If last prinf() was more than 1sec ago
		// printf and reset
		sprintf(frameTimeBuffer, "%.2f ms/frame\n", 1000.0 / double(nbFrames));
		nbFrames = 0;
		lastTime += 1.0;
	}

	debugDisplay->addDebug(frameTimeBuffer);

}