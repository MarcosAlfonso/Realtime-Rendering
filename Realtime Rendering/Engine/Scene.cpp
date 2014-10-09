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

std::shared_ptr<BaseEntity> physicsSphere;
std::shared_ptr<BaseEntity> skySphere;
std::shared_ptr<BaseEntity> groundCube;
std::shared_ptr<BaseEntity> terrain;
std::shared_ptr<BaseEntity> mainCamera;

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
extern std::shared_ptr<Mesh> suzanne;
extern std::shared_ptr<Mesh> sphere;
extern std::shared_ptr<Mesh> cube;
extern std::shared_ptr<GridMesh> grid;

extern Hierarchy* hierarchy;

int ID_Count = 0;

void Scene::AddEntity(std::shared_ptr<BaseEntity> ent)
{
	//Unique ID for all objects in scene (probably needs testing)
	ent->ID = ID_Count;

	//Add to Entities list
	GameEntities.push_back(ent);

	//Add an entry to the Hierarchy and set up selection and shiz
	CEGUI::ListboxTextItem * newItem = new CEGUI::ListboxTextItem(ent->Name, ID_Count);
	newItem->setUserData(ent.get());
	newItem->setSelectionColours(CEGUI::Colour(1, 1, 1, .2));
	newItem->setSelectionBrushImage("TaharezLook/GenericBrush");
	hierarchy->Listbox->addItem(newItem);

	//Increment Unique ID
	ID_Count++;

}


Scene::Scene()
{
	skySphere = std::shared_ptr<BaseEntity>(new BaseEntity("Sky Sphere"));
	skySphere->addComponent(std::shared_ptr<RenderComponent>(new RenderComponent(skySphere, sphere, GradientShaderID, skySphereTexture)));
	skySphere->Transform->setScale(1000, 1000, 1000);
	AddEntity(skySphere);


	terrain = std::shared_ptr<BaseEntity>(new BaseEntity("Terrain"));
	std::shared_ptr<RenderComponent> terrainRender = std::shared_ptr<RenderComponent>(new RenderComponent(terrain, grid, StandardShaderID, GrassTexture));
	terrainRender->flipCullFace = true;
	terrain->addComponent(std::shared_ptr<RenderComponent>(terrainRender));
	terrain->Transform->setScale(-1, 1, 1);
	terrain->Transform->setRotation(0, glm::half_pi<float>(), 0);
	terrain->addComponent(std::shared_ptr<PhysicsComponent>(new PhysicsComponent(terrain, TERRAIN, 0, grid->heightFieldArray)));
	AddEntity(terrain);

	
	mainCamera = std::shared_ptr<BaseEntity>(new BaseEntity("Main Camera"));
	mainCamera->addComponent(std::shared_ptr<CameraComponent>(new CameraComponent(mainCamera)));
	mainCamera->Transform->setPosition(2, 1, 8);
	std::shared_ptr<CameraComponent> cam = std::static_pointer_cast<CameraComponent>(mainCamera->getComponent(CAMERA));
	cam->horizontalAngle = 3.14;
	AddEntity(mainCamera);

	
}


Scene::~Scene()
{

}
