#pragma once
#include "Scene.h"

#include "InputManager.h"
#include "Entities\BaseEntity.h"

#include "Components\RenderComponent.h"
#include "Components\PhysicsComponent.h"
#include "Components\\CameraComponent.h"

#include "Graphics/GridMesh.h"

#include "GUI/Hierarchy.h"
#include "GUI/Stats.h"

#include <glm/gtc/constants.hpp>
#include <CEGUI/CEGUI.h>
#include "Entities/FreeCameraInput.h"

BaseEntity * physicsSphere;
BaseEntity * skySphere;
BaseEntity * groundCube;
BaseEntity * terrain;
BaseEntity * mainCamera;
BaseEntity * tessTest;

// Create and compile our GLSL program from the shaders
extern GLuint StandardShaderID;
extern GLuint FullbrightShaderID;
extern GLuint DebugLineShaderID;
extern GLuint GradientShaderID;
extern GLuint TessTerrainShaderID;
;


// Load the texture
extern GLuint GridTexture;
extern GLuint CloudTexture;
extern GLuint skySphereTexture;
extern GLuint GrassTexture;

//Models are loaded from .obj's, changed extension to .model to avoid linker issues with VS
extern Mesh * suzanne;
extern Mesh * sphere;
extern Mesh * cube;
extern GridMesh * terrainGrid;
extern GridMesh * tessGrid;

extern Hierarchy* hierarchy;

CEGUI::ListboxTextItem * newItem;
int ID_Count = 0;

void Scene::AddEntity(BaseEntity * ent)
{
	//Add to Entities list
	GameEntities.push_back(ent);

	hierarchy->Listbox->addItem(ent->hierarchyItem);

	//Increment Unique ID
	ID_Count++;

}


Scene::Scene()
{
	LoadScene();
}

void Scene::LoadScene()
{
	
	skySphere = new BaseEntity("Sky Sphere");
	skySphere->addComponent(new RenderComponent(skySphere, sphere, GradientShaderID, skySphereTexture, false));
	skySphere->Transform->setScale(1000, 1000, 1000);
	AddEntity(skySphere);

	/*
	terrain = new BaseEntity("Terrain");
	auto terrainRender = new RenderComponent(terrain, terrainGrid, StandardShaderID, GrassTexture);
	terrainRender->flipCullFace = true;
	terrain->addComponent(terrainRender);
	terrain->Transform->setScale(-1, 1, 1);
	terrain->Transform->setRotation(0, glm::half_pi<float>(), 0);
	terrain->addComponent(new PhysicsComponent(terrain, TERRAIN, 0, terrainGrid->heightFieldArray));
	AddEntity(terrain);
	*/
	
	tessTest = new BaseEntity("tess Test");
	tessTest->addComponent(new RenderComponent(tessTest, tessGrid, TessTerrainShaderID, GridTexture, true));
	AddEntity(tessTest);
	
	mainCamera = new BaseEntity("Main Camera");
	mainCamera->addComponent(new CameraComponent(mainCamera));
	mainCamera->Transform->setPosition(2, 1, 8);
	auto cam = mainCamera->getElementOfType<CameraComponent>();
	cam->horizontalAngle = 3.14;
	addInput(new FreeCameraInput(mainCamera));
	AddEntity(mainCamera);
	
}


Scene::~Scene()
{
	for (int i = 0; i < GameEntities.size(); i++)
	{
		delete(GameEntities[i]);
	}

	delete(newItem);
}
