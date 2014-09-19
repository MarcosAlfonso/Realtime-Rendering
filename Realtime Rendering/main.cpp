//Include external project files
#include "Engine/Input.h"
#include "Engine/Graphics/GridMesh.h"
#include "Engine/Graphics/Mesh.h"
#include "Engine/Util/loadShader.h"
#include "Engine/Util/textureLoad.h"
#include "Engine/Graphics/bulletDebugDraw.h"
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

#include <btBulletCollisionCommon.h>
#include <btBulletDynamicsCommon.h>

#include <CEGUI/CEGUI.h>
#include <CEGUI/RendererModules/OpenGL/GL3Renderer.h>

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
int nbFrames = 0;

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

//Physics
btDiscreteDynamicsWorld* dynamicsWorld;
btDefaultCollisionConfiguration* collisionConfiguration;
btCollisionDispatcher* dispatcher;
btBroadphaseInterface* overlappingPairCache;
btSequentialImpulseConstraintSolver* solver;
bulletDebugDraw* drawer;


#pragma endregion 

int main(void)
{
	SetupConfiguration();

	InitializeBullet();

	LoadAssets();
	
	do{

		CalculateFrameTime();

		ControlsUpdate();

		Render();
		
		dynamicsWorld->stepSimulation(1 / 60.f, 10);	

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

	InitializeCEGUI();

	ControlInit();
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

	CEGUI::System::getSingleton().renderAllGUIContexts();	

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

	//Physics
	CleanupBullet();
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
	
	//drawer = new bulletDebugDraw();

	//dynamicsWorld->setDebugDrawer(drawer); 
	//dynamicsWorld->getDebugDrawer()->setDebugMode(btIDebugDraw::DBG_DrawWireframe);
}

void InitializeCEGUI()
{
	CEGUI::OpenGL3Renderer& myRenderer = CEGUI::OpenGL3Renderer::bootstrapSystem();

	CEGUI::DefaultResourceProvider* rp = static_cast<CEGUI::DefaultResourceProvider*>
		(CEGUI::System::getSingleton().getResourceProvider());


	rp->setResourceGroupDirectory("schemes", "Assets/CEGUI/schemes/");
	rp->setResourceGroupDirectory("imagesets", "Assets/CEGUI/imagesets/");
	rp->setResourceGroupDirectory("fonts", "Assets/CEGUI/fonts/");
	rp->setResourceGroupDirectory("layouts", "Assets/CEGUI/layouts/");
	rp->setResourceGroupDirectory("looknfeels", "Assets/CEGUI/looknfeel/");
	rp->setResourceGroupDirectory("lua_scripts", "Assets/CEGUI/lua_scripts/");

	CEGUI::ImageManager::setImagesetDefaultResourceGroup("imagesets");
	CEGUI::Font::setDefaultResourceGroup("fonts");
	CEGUI::Scheme::setDefaultResourceGroup("schemes");
	CEGUI::WidgetLookManager::setDefaultResourceGroup("looknfeels");
	CEGUI::WindowManager::setDefaultResourceGroup("layouts");
	CEGUI::ScriptModule::setDefaultResourceGroup("lua_scripts");

	CEGUI::SchemeManager::getSingleton().createFromFile("TaharezLook.scheme");

	
	CEGUI::WindowManager& wmgr = CEGUI::WindowManager::getSingleton();

	CEGUI::Window* myRoot = wmgr.createWindow("DefaultWindow", "root");
	CEGUI::System::getSingleton().getDefaultGUIContext().setRootWindow(myRoot);

	CEGUI::FrameWindow* fWnd = static_cast<CEGUI::FrameWindow*>(
		wmgr.createWindow("TaharezLook/FrameWindow", "testWindow"));

	myRoot->addChild(fWnd);

	// position a quarter of the way in from the top-left of parent.
	fWnd->setPosition(CEGUI::UVector2(CEGUI::UDim(0.25f, 0.0f), CEGUI::UDim(0.25f, 0.0f)));
	// set size to be half the size of the parent
	fWnd->setSize(CEGUI::USize(CEGUI::UDim(0.25f, 0.0f), CEGUI::UDim(0.25f, 0.0f)));

	fWnd->setText("Hello World!");
	
}

void CleanupBullet()
{

	 delete(collisionConfiguration);
	 delete(dispatcher);
	 delete(overlappingPairCache);
	 delete(solver);
	 delete(dynamicsWorld);
}