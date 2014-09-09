// Include standard headers
#include <vector>
#include <iostream>

#include <btBulletCollisionCommon.h>
#include <btBulletDynamicsCommon.h>

//Uncomment for leak detection
//#include <vld.h>

// Include GLEW
#define GLEW_STATIC
#include <GL/glew.h>

// Include GLFW
#include <GLFW/glfw3.h>
GLFWwindow* window;

// Include GLM
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>

//Include external project files
#include "Common/Util/loadShader.h"
#include "Common/Util/textureLoad.h"
#include "Common/controls.h"
#include "Common/Graphics/mesh.h"
#include "Common/Graphics/GridMesh.h"
#include "Common/Graphics/bulletDebugDraw.h"
#include "Common/Util/text2D.h"
#include "Common/Util/DebugDisplay.h"
#include "Common/EngineObjects/GameEntity.h"
#include "Common/EngineObjects/Components/RenderComponent.h"
#include "Common/EngineObjects/Components/PhysicsComponent.h"

#pragma region Declarations
void SetupConfiguration();
void LoadAssets();
void Render();
void CleanupMemory();
void CalculateFrameTime();
void InitializeBullet();
void BulletStep();
void CleanupBullet();

//Debug String
char debugBuffer[512] = "";
char frameTimeBuffer[128] = "";
int vertexCount;

double lastTime;
int nbFrames = 0;

//Window
bool shouldClose;

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

//Debug
DebugDisplay * debugDisplay;
DebugDisplay * timedDebugDisplay;

//GameEntities
std::vector<GameEntity*> GameEntities;
GameEntity* physicsSphere;
GameEntity* physicsSphere2;
GameEntity* physicsSphere3;
GameEntity* skySphere;
GameEntity* groundCube;

//Physics
btDiscreteDynamicsWorld* dynamicsWorld;
btDefaultCollisionConfiguration* collisionConfiguration;
btCollisionDispatcher* dispatcher;
btBroadphaseInterface* overlappingPairCache;
btSequentialImpulseConstraintSolver* solver;


#pragma endregion 

int main(void)
{
	SetupConfiguration();

	InitializeBullet();

	LoadAssets();
	
	do{

		CalculateFrameTime();
		
		Render();

		dynamicsWorld->stepSimulation(1 / 60.f, 10);

		glfwPollEvents();

	} // Check if the ESC key was pressed or the window was closed
	while (!shouldClose && glfwWindowShouldClose(window) == 0);

	CleanupMemory();

	glfwTerminate();

	return 0;
}

void SetupConfiguration()
{
	//frameTime Update
	lastTime = glfwGetTime();

	//Text Debuggers
	debugDisplay = new DebugDisplay(glm::vec2(10, 565), false);
	timedDebugDisplay = new DebugDisplay(glm::vec2(525, 565), true);

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

	ControlInit();
}

void LoadAssets()
{
	//Initialize Text
	initText2D("Assets/DroidSansMonoSmall.dds");

	// Create and compile our GLSL program from the shaders
	StandardShaderID = CreateShaderProgram("Shaders/standard.vert", "Shaders/standard.frag", NULL);
	FullbrightShaderID = CreateShaderProgram("Shaders/fullbright.vert", "Shaders/fullbright.frag", NULL);
	debugLineShaderID = CreateShaderProgram("Shaders/debugLine.vert", "Shaders/debugLine.frag", NULL);

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
	grid = new GridMesh(100, 100, 2, 2);


	skySphere = new GameEntity();
	skySphere->addComponent(new RenderComponent(skySphere, sphere, FullbrightShaderID, skySphereTexture));
	skySphere->Transform->setScale(glm::vec3(100, -100, 100));
	GameEntities.push_back(skySphere);

	groundCube = new GameEntity();
	groundCube->addComponent(new RenderComponent(groundCube, sphere, StandardShaderID, GridTexture));
	PhysicsComponent* phys = new PhysicsComponent(groundCube, SPHERE, btVector3(1, 1, 1), 0, NULL);
	groundCube->addComponent(phys);

	groundCube->Transform->setScale(glm::vec3(1,1,1));
	phys->SetPosition(glm::vec3(0, 5, 0));

	GameEntities.push_back(groundCube);

}

void Render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	computeMatricesFromInputs();

	vertexCount = 0;

	for (int i = 0; i < GameEntities.size(); i++)
	{
		GameEntities[i]->Update();
	}

	sprintf(debugBuffer, "Vertex Count\n: %d", vertexCount);
	debugDisplay->addDebug(debugBuffer);
	dynamicsWorld->debugDrawWorld();

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

	// Delete the text's VBO, the shader and the texture
	cleanupText2D();

	//Physics
	CleanupBullet();

	//Delete debuggers
	delete(debugDisplay);
	delete(timedDebugDisplay);
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

void InitializeBullet()
{
	
	///collision configuration contains default setup for memory, collision setup. Advanced users can create their own configuration.
	collisionConfiguration = new btDefaultCollisionConfiguration();

	///use the default collision dispatcher. For parallel processing you can use a diffent dispatcher (see Extras/BulletMultiThreaded)
	dispatcher = new	btCollisionDispatcher(collisionConfiguration);

	///btDbvtBroadphase is a good general purpose broadphase. You can also try out btAxis3Sweep.
	overlappingPairCache = new btDbvtBroadphase();

	///the default constraint solver. For parallel processing you can use a different solver (see Extras/BulletMultiThreaded)
	solver = new btSequentialImpulseConstraintSolver;

	//Creates Bullet world with the stuff referenced 
	dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, overlappingPairCache, solver, collisionConfiguration);
	dynamicsWorld->setGravity(btVector3(0, -10, 0));


	bulletDebugDraw* drawer = new bulletDebugDraw();

	dynamicsWorld->setDebugDrawer(drawer); 
	dynamicsWorld->getDebugDrawer()->setDebugMode(btIDebugDraw::DBG_DrawWireframe);
}

void CleanupBullet()
{

	 delete(collisionConfiguration);
	 delete(dispatcher);
	 delete(overlappingPairCache);
	 delete(solver);
	 delete(dynamicsWorld);
}