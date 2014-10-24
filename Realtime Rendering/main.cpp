//Include external project files
#include "Engine/Graphics/framebuffer.h"
#include "Engine/GUI/Stats.h"
#include "Engine/AssetManager.h"
#include "Engine/SceneManager.h"
#include "Engine/PhysicsManager.h"
#include "Engine/GUIManager.h"
#include "Engine/InputManager.h"
#include "Engine/Graphics/GridMesh.h"
#include "Engine/Graphics/Mesh.h"
#include "Engine/Entities/BaseEntity.h"
#include "Engine/Components/RenderComponent.h"
#include "Engine/Components/PhysicsComponent.h"
#include "Engine/Components/CameraComponent.h"

#include "Libraries/include/pcre.h"

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

#include <CEGUI/CEGUI.h>
#include "Engine/Graphics/bulletDebugDraw.h"
#include "Engine/GUI/HelpBox.h"

//#include <vld.h>

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
double curTime;
float DeltaTime;
float printDeltaTime;

//Window
int screenX = 1920;
int screenY = 1080;
extern bool doRenderGui;

extern bulletDebugDraw * drawer;
extern btDiscreteDynamicsWorld * dynamicsWorld;

extern PhysicsComponent * selectedObjectPhys;

extern GLuint FramebufferShaderID;
extern GLuint screenUvMapTexture;

Scene * scene;

extern Stats * stats;
extern HelpBox * helpbox;

Framebuffer * frameBufferTest;



#pragma endregion 

int main(void)
{
	SetupConfiguration();

	LoadAssets();
	
	do{

		CalculateFrameTime();

		UpdatePhysics();

		Render();

		UpdateInput();

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
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);


	// Open a window and create its OpenGL context
	window = glfwCreateWindow(screenX, screenY, "Realtime Rendering Engine", glfwGetPrimaryMonitor(), nullptr);

	if (window == NULL){
		fprintf(stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n");
		glfwTerminate();
	}
	glfwMakeContextCurrent(window);

	// Initialize GLEW7
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
	InitializeAssets();
		
	scene =  new Scene();
	LoadScene(scene);
}

void Render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	vertexCount = 0;

	UpdateScene();

	//Debug printing
	sprintf(debugBuffer, "Delta Time: %fms\n", DeltaTime * 1000);
	stats->Label->appendText(debugBuffer);

	sprintf(debugBuffer, "Vertex Count: %d\n", vertexCount);
	stats->Label->appendText(debugBuffer);
	
	if (doRenderGui)
	{
		RenderGUI();
	}

	
	//Renders selected object with phys wireframe (uses old school render...)
	//dynamicsWorld->debugDrawWorld();

	//Because CEGUI demands it
	glDisable(GL_BLEND);

	glfwSwapBuffers(window);

	//Clears stats label for next frame
	stats->Label->setText("");
}

void CleanupMemory()
{
	CleanupAssets();
	CleanupScene();
	CleanupPhysics();
	CleanupGUI();
}

void CalculateFrameTime()
{
	lastTime = curTime;
	curTime = glfwGetTime();
	DeltaTime = float(curTime - lastTime);


}

