#pragma once
#include "Scene.h"

#include "Entities\BaseEntity.h"
#include "Entities\FreeCamera.h"

#include "Components\RenderComponent.h"
#include "Components\PhysicsComponent.h"

#include "Graphics/GridMesh.h"

#include "GUI/Hierarchy.h"
#include "GUI/Stats.h"

#include <glm/gtc/constants.hpp>
#include <CEGUI/CEGUI.h>

BaseEntity* physicsSphere;
BaseEntity* skySphere;
BaseEntity* groundCube;
BaseEntity* terrain;
FreeCamera * mainCamera;

// Create and compile our GLSL program from the shaders
extern GLuint StandardShaderID;
extern GLuint FullbrightShaderID;
extern GLuint DebugLineShaderID;
extern GLuint GradientShaderID;


// Load the texture
extern GLuint GridTexture;
extern GLuint CloudTexture;
extern GLuint skySphereTexture;
extern GLuint GrassTexture;

//Models are loaded from .obj's, changed extension to .model to avoid linker issues with VS
extern Mesh * suzanne;
extern Mesh * sphere;
extern Mesh * cube;
extern GridMesh * grid;

extern Hierarchy* hierarchy;

int ID_Count = 0;

void Scene::AddEntity(BaseEntity * ent)
{
	//Unique ID for all objects in scene (probably needs testing)
	ent->ID = ID_Count;

	//Add to Entities list
	GameEntities.push_back(ent);

	//Add an entry to the Hierarchy and set up selection and shiz
	CEGUI::ListboxTextItem * newItem = new CEGUI::ListboxTextItem(ent->Name, ID_Count);
	newItem->setUserData(ent);
	newItem->setSelectionColours(CEGUI::Colour(1, 1, 1, .2));
	newItem->setSelectionBrushImage("TaharezLook/GenericBrush");
	hierarchy->Listbox->addItem(newItem);

	//Increment Unique ID
	ID_Count++;
}


Scene::Scene()
{
	skySphere = new BaseEntity("Sky Sphere");
	skySphere->addComponent(new RenderComponent(skySphere, sphere, GradientShaderID, skySphereTexture));
	skySphere->Transform->setScale(1000, 1000, 1000);
	AddEntity(skySphere);


	terrain = new BaseEntity("Terrain");
	RenderComponent * terrainRender = new RenderComponent(terrain, grid, StandardShaderID, GrassTexture);
	terrainRender->flipCullFace = true;
	terrain->addComponent(terrainRender);
	terrain->Transform->setScale(-1, 1, 1);
	terrain->Transform->setRotation(0, glm::half_pi<float>(), 0);
	terrain->addComponent(new PhysicsComponent(terrain, TERRAIN, 0, grid->heightFieldArray));
	AddEntity(terrain);

	mainCamera = new FreeCamera("Main Camera");
	mainCamera->Transform->setPosition(2, 1, 8);
	mainCamera->Camera->horizontalAngle = 3.14f;
	AddEntity(mainCamera);
}


Scene::~Scene()
{
}
