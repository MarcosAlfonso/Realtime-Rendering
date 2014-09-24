//Include external project files
#include "Engine/PhysicsManager.h"
#include "Engine/GUIManager.h"
#include "Engine/InputManager.h"
#include "Engine/Graphics/GridMesh.h"
#include "Engine/Graphics/Mesh.h"
#include "Engine/Util/loadShader.h"
#include "Engine/Util/textureLoad.h"
#include "Engine/Entities/BaseEntity.h"
#include "Engine/Components/RenderComponent.h"
#include "Engine/Components/PhysicsComponent.h"
#include "Engine/Components/CameraComponent.h"
#include "Engine/Entities/FreeCamera.h"

// Include GLEW
#define GLEW_STATIC
#include <GL/glew.h>

// Include GLFW
#include <GLFW/glfw3.h>
GLFWwindow* window;

// Include GLM
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>

// Include standard headers
#include <vector>
#include <iostream>

#pragma region Declarations
void SetupConfiguration();
void LoadAssets();
void Render();
void CleanupMemory();
void CalculateFrameTime();
void InitializeBullet();
void InitializeCEGUI();
void BulletStep();
void CleanupBullet();

//Debug String
char debugBuffer[512] = "";
char frameTimeBuffer[128] = "";
int vertexCount;

double lastTime;
double curTime;
float DeltaTime;

// Create and compile our GLSL program from the shaders
GLuint StandardShaderID;
GLuint FullbrightShaderID;
GLuint debugLineShaderID;

// Load the texture
GLuint GridTexture;
GLuint CloudTexture;
GLuint skySphereTexture;
GLuint GrassTexture;

//Models are loaded from .obj's, changed extension to .model to avoid linker issues with VS
//Suzanne
Mesh * suzanne;
Mesh * sphere;
Mesh * cube;

//Grid Mesh
GridMesh * grid;

//GameEntities
std::vector<BaseEntity*> GameEntities;
BaseEntity* physicsSphere;
BaseEntity* skySphere;
BaseEntity* groundCube;
BaseEntity* terrain;
FreeCamera * mainCamera;

#pragma endregion 

int main(void)
{
	SetupConfiguration();

	LoadAssets();
	
	do{

		CalculateFrameTime();

		Render();

		UpdateInput();

		UpdatePhysics();

		glfwPollEvents();

	} // Check if the ESC key was pressed or the window was closed
	while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS && glfwWindowShouldClose(window) == 0);

	CleanupMemory();

	glfwTerminate();

	return 0;
}

void SetupConfiguration()
{
	//frameTime Update
	lastTime = glfwGetTime();
	curTime = glfwGetTime();

	// Initialize GLFW
	if (!glfwInit())
	{
		fprintf(stderr, "Failed to initialize GLFW\n");
	}

	glfwWindowHint(GLFW_SAMPLES, 1);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);


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

	InitializeGUI();

	InitializePhysics();

	InitializeInput();
}

void LoadAssets()
{
	// Create and compile our GLSL program from the shaders
	StandardShaderID = CreateShaderProgram("Engine/Shaders/standard.vert", "Engine/Shaders/standard.frag", NULL);
	FullbrightShaderID = CreateShaderProgram("Engine/Shaders/fullbright.vert", "Engine/Shaders/fullbright.frag", NULL);
	debugLineShaderID = CreateShaderProgram("Engine/Shaders/debugLine.vert", "Engine/Shaders/debugLine.frag", NULL);

	// Load the texture
	GridTexture = loadDDS("Assets/GridTexture.dds");
	CloudTexture = loadDDS("Assets/CloudTexture.dds");
	skySphereTexture = loadDDS("Assets/skySphere.dds");
	GrassTexture = loadDDS("Assets/grass.dds");

	//Models are loaded from .obj's, changed extension to .model to avoid linker issues with VS
	//Suzanne
	suzanne = new Mesh();
	suzanne->loadFromFile("Assets/suzanne.model");
	//Sphere
	sphere = new Mesh();
	sphere->loadFromFile("Assets/sphere.model");
	//Sphere
	cube = new Mesh();
	cube->loadFromFile("Assets/cube.model");
	//Terrain
	grid = new GridMesh(30, 30, 2, 2);


	skySphere = new BaseEntity("Sky Sphere");
	skySphere->addComponent(new RenderComponent(skySphere, sphere, FullbrightShaderID, skySphereTexture));
	skySphere->Transform->setScale(100, -100, 100);
	GameEntities.push_back(skySphere);


	terrain = new BaseEntity("Terrain");
	RenderComponent * terrainRender = new RenderComponent(terrain, grid, StandardShaderID, GrassTexture);
	terrainRender->flipCullFace = true;
	terrain->addComponent(terrainRender);
	terrain->Transform->setScale(-1, 1, 1);
	terrain->Transform->setRotation(0, glm::half_pi<float>(), 0);

	terrain->addComponent(new PhysicsComponent(terrain, TERRAIN, 0, grid->heightFieldArray));
	GameEntities.push_back(terrain);

	mainCamera = new FreeCamera("Main Camera");
	mainCamera->Transform->setPosition(2, 1, 8);
	mainCamera->Camera->horizontalAngle = 3.14f;
	GameEntities.push_back(mainCamera);
}

void Render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	vertexCount = 0;

	for (int i = 0; i < GameEntities.size(); i++)
	{
		GameEntities[i]->Update();
	}

	RenderGUI();

	glDisable(GL_BLEND);
	
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
	glDeleteTextures(1, &GrassTexture);

	//Delete Loaded Meshes
	delete(suzanne);
	delete(sphere);
	delete(grid);

	//Delete Entities
	for (int i = 0; i < GameEntities.size(); i++)
	{
		delete(GameEntities[i]);
	}

	CleanupPhysics();
}

void CalculateFrameTime()
{
	lastTime = curTime;
	curTime = glfwGetTime();
	DeltaTime = float(curTime - lastTime);

}

