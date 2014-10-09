#include "AssetManager.h"
#include <memory>

// Create and compile our GLSL program from the shaders
GLuint StandardShaderID;
GLuint FullbrightShaderID;
GLuint DebugLineShaderID;
GLuint GradientShaderID;

// Load the texture
GLuint GridTexture;
GLuint CloudTexture;
GLuint skySphereTexture;
GLuint GrassTexture;

//Models are loaded from .obj's, changed extension to .model to avoid linker issues with VS
//Suzanne
std::shared_ptr<Mesh> suzanne;
std::shared_ptr<Mesh> sphere;
std::shared_ptr<Mesh> cube;

//Grid Mesh
std::shared_ptr<GridMesh> grid;

void InitializeAssets()
{
	// Create and compile our GLSL program from the shaders
	StandardShaderID = CreateShaderProgram("Engine/Shaders/standard.vert", "Engine/Shaders/standard.frag", NULL);
	FullbrightShaderID = CreateShaderProgram("Engine/Shaders/fullbright.vert", "Engine/Shaders/fullbright.frag", NULL);
	DebugLineShaderID = CreateShaderProgram("Engine/Shaders/debugLine.vert", "Engine/Shaders/debugLine.frag", NULL);
	GradientShaderID = CreateShaderProgram("Engine/Shaders/gradient.vert", "Engine/Shaders/gradient.frag", NULL);

	// Load the texture
	GridTexture = loadDDS("Assets/GridTexture.dds");
	CloudTexture = loadDDS("Assets/CloudTexture.dds");
	skySphereTexture = loadDDS("Assets/skySphere.dds");
	GrassTexture = loadDDS("Assets/grass.dds");

	//Models are loaded from .obj's, changed extension to .model to avoid linker issues with VS
	//Suzanne
	suzanne = std::shared_ptr<Mesh>(new Mesh());
	suzanne->loadFromFile("Assets/suzanne.model");
	//Sphere
	sphere = suzanne = std::shared_ptr<Mesh>(new Mesh());
	sphere->loadFromFile("Assets/sphere.model");
	//Sphere
	cube = suzanne = std::shared_ptr<Mesh>(new Mesh());
	cube->loadFromFile("Assets/cube.model");
	//Terrain
	grid = std::shared_ptr<GridMesh>(new GridMesh(30, 30, 2, 2));

}

void CleanupAssets()
{
	//Cleanup Shaders
	glDeleteProgram(StandardShaderID);
	glDeleteProgram(FullbrightShaderID);

	//Delete Textures
	glDeleteTextures(1, &GridTexture);
	glDeleteTextures(1, &FullbrightShaderID);
	glDeleteTextures(1, &CloudTexture);
	glDeleteTextures(1, &GrassTexture);

}