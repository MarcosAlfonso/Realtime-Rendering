#include "AssetManager.h"


// Create and compile our GLSL program from the shaders
GLuint StandardShaderID;
GLuint FullbrightShaderID;
GLuint DebugLineShaderID;
GLuint GradientShaderID;
GLuint FramebufferShaderID;
GLuint TessTerrainShaderID;

// Load the texture
GLuint GridTexture;
GLuint CloudTexture;
GLuint skySphereTexture;
GLuint GrassTexture;
GLuint screenUvMapTexture;
GLuint heightMapTexture;

//Models are loaded from .obj's, changed extension to .model to avoid linker issues with VS
//Suzanne
Mesh * suzanne;
Mesh * sphere;
Mesh * cube;

//Grid Mesh
GridMesh * terrainGrid;
GridMesh * tessGrid;

void InitializeAssets()
{
	// Create and compile our GLSL program from the shaders
	StandardShaderID = CreateShaderProgram("Engine/Shaders/standard.vert", NULL, NULL, NULL, "Engine/Shaders/standard.frag");
	FullbrightShaderID = CreateShaderProgram("Engine/Shaders/fullbright.vert", NULL, NULL, NULL, "Engine/Shaders/fullbright.frag");
	DebugLineShaderID = CreateShaderProgram("Engine/Shaders/debugLine.vert", NULL, NULL, NULL, "Engine/Shaders/debugLine.frag");
	GradientShaderID = CreateShaderProgram("Engine/Shaders/gradient.vert", NULL, NULL, NULL, "Engine/Shaders/gradient.frag");
	FramebufferShaderID = CreateShaderProgram("Engine/Shaders/framebuffer.vert", NULL, NULL, NULL, "Engine/Shaders/framebuffer.frag");
	TessTerrainShaderID = CreateShaderProgram("Engine/Shaders/terrain/terrain.vert",  "Engine/Shaders/terrain/terrain.tessCntrl", "Engine/Shaders/terrain/terrain.tessEval", "Engine/Shaders/terrain/terrain.geom",  "Engine/Shaders/terrain/terrain.frag");

	// Load the texture
	GridTexture = loadDDS("Assets/GridTexture.dds");
	CloudTexture = loadDDS("Assets/CloudTexture.dds");
	skySphereTexture = loadDDS("Assets/skySphere.dds");
	GrassTexture = loadDDS("Assets/grass.dds");
	screenUvMapTexture = loadDDS("Assets/uvmap.dds");
	heightMapTexture = loadDDS("Assets/terrainHeightMap.dds");

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
	terrainGrid = new GridMesh(250, 250, 2, 2, true);
	tessGrid = new GridMesh(2, 2, 200, 200, false);
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
	delete(terrainGrid);

}