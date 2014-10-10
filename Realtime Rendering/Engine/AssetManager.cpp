#include "AssetManager.h"


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
Mesh * suzanne;
Mesh * sphere;
Mesh * cube;

//Grid Mesh
GridMesh * grid;

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
	
	delete(cube);
	delete(sphere);
	delete(suzanne);
	delete(grid);

}