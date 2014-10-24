#pragma once
#include "InputManager.h"

#include "GUI/Console.h"
#include "GUI/HelpBox.h"
#include "GUI/Hierarchy.h"
#include "GUI/Stats.h"
#include "SceneManager.h"
#include "Entities/BaseEntity.h"
#include "Components/RenderComponent.h"
#include "Components/PhysicsComponent.h"
#include "Components/CameraComponent.h"

#define GLEW_STATIC
#include <gl/glew.h>
#include <GLFW/glfw3.h>

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

//Bullet Physics
#include <btBulletCollisionCommon.h>
#include <btBulletDynamicsCommon.h>

// Include GLFW
extern GLFWwindow* window; 

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <CEGUI/CEGUI.h>
#include "Scene.h"


extern std::vector<BaseEntity> GameEntities;

extern Mesh * cube;
extern Mesh * sphere;
extern GLuint GridTexture;
extern GLuint StandardShaderID;
extern float lightRot;

extern int screenX;
extern int screenY;


extern btDiscreteDynamicsWorld* dynamicsWorld;

extern char debugBuffer[];
extern bool doRenderGui;

extern HelpBox* inspector;
extern Hierarchy* hierarchy;
extern Stats* stats;
extern Console* console;

bool shiftHeldDown;
std::vector<InputComponent*> InputList;

CEGUI::MouseButton GlfwToCeguiButton(int glfwButton);
CEGUI::Key::Scan GlfwToCeguiKey(int glfwKey);
int charCheck(int glfwKey);

PhysicsComponent * selectedObjectPhys;

extern BaseEntity * mainCamera;

extern Scene * scene;

extern glm::vec3 groundColor;
extern glm::vec3 skyColor;

extern int ID_Count;

bool pausePhysics;




//Input Manager, handles the Input system

void addInput(InputComponent * input)
{
	InputList.push_back(input);
}

void KeyboardCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	//If typing inside an editbox, skip to CEGUI Section
	if (!console->editbox->isActive())
	{
		//Deletes selected object
		if (key == GLFW_KEY_DELETE && action == GLFW_PRESS && selectedObjectPhys != nullptr)
		{
			for (int i = 0; i < scene->GameEntities.size(); i++)
			{
				if (selectedObjectPhys == scene->GameEntities[i]->getElementOfType<PhysicsComponent>())
				{
					selectedObjectPhys = nullptr;

					auto objectToDelete = scene->GameEntities.at(i);
					hierarchy->Listbox->removeItem(objectToDelete->hierarchyItem);

					scene->GameEntities.erase(scene->GameEntities.begin() + i);
					delete(objectToDelete);
				}
			}
		}
		
		//Q Spanws a Physics Sphere
		if (key == GLFW_KEY_Q && action == GLFW_PRESS)
		{
			BaseEntity * physicsSphere = new BaseEntity("Physics Sphere");
			physicsSphere->Transform->setPosition(0, 5, 0);

			physicsSphere->addComponent(new RenderComponent(physicsSphere, sphere, StandardShaderID, GridTexture, false));
			physicsSphere->addComponent(new PhysicsComponent(physicsSphere, SPHERE, 1, std::vector<float>()));
			scene->AddEntity(physicsSphere);
			console->listbox->addItem(new CEGUI::ListboxTextItem("physicsSphere Created"));
		}

		//P Pauses Physics the power!
		if (key == GLFW_KEY_P && action == GLFW_PRESS)
		{
			pausePhysics = !pausePhysics;
		}

		//E Spanws a Physics Sphere
		if (key == GLFW_KEY_E && action == GLFW_PRESS)
		{
			BaseEntity * physicsSphere = new BaseEntity("Physics Cube");
			physicsSphere->Transform->setPosition(0, 5, 0);

			physicsSphere->addComponent(new RenderComponent(physicsSphere, cube, StandardShaderID, GridTexture, false));
			physicsSphere->addComponent(new PhysicsComponent(physicsSphere, BOX, 1, std::vector<float>()));
			scene->AddEntity(physicsSphere);
			console->listbox->addItem(new CEGUI::ListboxTextItem("physicsSphere Created"));
		}

		

		//G hides or show GUI
		if (key == GLFW_KEY_G && (action == GLFW_PRESS))
		{
			doRenderGui = !doRenderGui;
		}



		//InputComponent Keyboards Callbacks
		for (int i = 0; i < InputList.size(); i++)
		{
			InputList[i]->KeyboardInputCallback(window, key, scancode, action, mods);
		}
	}

	//Up arrow copies last command to console editbox
	if (key == GLFW_KEY_UP && action == GLFW_PRESS)
	{
		console->editbox->setText(console->lastCommand);
		console->editbox->activate();
		console->editbox->setCaretIndex(999);
	}
	
	//CEGUI Keyboard conversion
	CEGUI::Key::Scan CEGUIKey = GlfwToCeguiKey(key);

	//If key is pressed or repeating
	if (action == GLFW_PRESS || action == GLFW_REPEAT)
	{
		//And the key isn't an unknown key send it as a key down injection
		if (CEGUIKey != CEGUI::Key::Unknown)
		{
			CEGUI::System::getSingleton().getDefaultGUIContext().injectKeyDown(CEGUIKey);
		}
		//If key is "unknown", it is probably a char
		else
		{
			//charCheck checks for capital letters, dead keys, and more
			int adjustedCharKey = charCheck(key);

			//If its a key we want to print, do it
			if (adjustedCharKey != -1)
				CEGUI::System::getSingleton().getDefaultGUIContext().injectChar(charCheck(key));
		}
	}

	//On key release
	if (action == GLFW_RELEASE)
	{
		//If not "unknown" (which really means its a char key most likely)
		if (CEGUIKey != CEGUI::Key::Unknown)
		{
			//Key release event
			CEGUI::System::getSingleton().getDefaultGUIContext().injectKeyUp(CEGUIKey);
		}
	}
}

void MouseButtonCallback(GLFWwindow * window, int button, int action, int mods)
{
	//Checks if mouse is over any core GUI windows
	bool overCEGUIWindow = console->myWindow->isMouseContainedInArea() || hierarchy->myWindow->isMouseContainedInArea() || stats->myWindow->isMouseContainedInArea();

	//Global Mouse Button Input

	//Left click press for ray casted selection, if not over a window
	
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS && !overCEGUIWindow)
	{
		double * mouseX = new double;
		double * mouseY = new double;

		glfwGetCursorPos(window, mouseX, mouseY);

		glm::vec3 out_origin;
		glm::vec3 out_direction;

		//Turn a click into a ray extending from camera to that point
		ScreenPosToWorldRay(
			(int)*mouseX, (int)*mouseY,             // Mouse position, in pixels, from bottom-left corner of the window
			screenX, screenY,  // Window size, in pixels
			mainCamera->getElementOfType<CameraComponent>()->ViewMatrix,               // Camera position and orientation
			mainCamera->getElementOfType<CameraComponent>()->ProjectionMatrix,         // Camera parameters (ratio, field of view, near and far planes)
			out_origin,              // Ouput : Origin of the ray. /!\ Starts at the near plane, so if you want the ray to start at the camera's position instead, ignore this.
			out_direction            // Ouput : Direction, in world space, of the ray that goes "through" the mouse.
			);

		//Make it really long
		out_direction = out_direction*1000.0f;

		//Sets up the ray casting function
		btCollisionWorld::ClosestRayResultCallback RayCallback(
			btVector3(out_origin.x, out_origin.y, out_origin.z),
			btVector3(out_direction.x, out_direction.y, out_direction.z)
			);
		
		//Cast ray into world
		dynamicsWorld->rayTest(
			btVector3(out_origin.x, out_origin.y, out_origin.z),
			btVector3(out_direction.x, out_direction.y, out_direction.z),
			RayCallback
			);

		/*
		int numItems = inspector->Listbox->getItemCount();

		//Clear inspector
		for (int i = numItems - 1; i >= 0; i--)
		{
			inspector->Listbox->removeItem(inspector->Listbox->getListboxItemFromIndex(i));
		}
		*/

		//If there is a hit
		if (RayCallback.hasHit()) {
			//Store pointer to the object hit
			void * hitPointer = RayCallback.m_collisionObject->getUserPointer();
			//Get the PhysicsComponent out of that pointer
			selectedObjectPhys = static_cast<PhysicsComponent*>(hitPointer);

			
			//Debug print to console
			console->logString(selectedObjectPhys->parentEntity->Name + " selected.");

			//Clear selection
			hierarchy->Listbox->clearAllSelections();

			//Pretty hacky I think, 
			for (int i = 0; i < hierarchy->Listbox->getItemCount(); i++)
			{
				//iterate through Listbox and check if selectedObject shares an ID with the currently selected object's ID
				if (hierarchy->Listbox->getListboxItemFromIndex(i)->getID() == selectedObjectPhys->parentEntity->ID)
				{
					//Match? That's the currently selected object
					hierarchy->Listbox->getListboxItemFromIndex(i)->setSelected(true);
				}
			}
		}
		else
		{
			//No hit? Nothing selected
			selectedObjectPhys = NULL;
		}
	}
	

	//InputComponent Mouse Button Callback
	for (int i = 0; i < InputList.size(); i++)
	{
		InputList[i]->MouseInputCallback(window, button, action, mods);
	}

	//CEGUI Mouse Button Injections
	CEGUI::MouseButton CEGUIMouseButton = GlfwToCeguiButton(button);

	//Inject Up or Down based on GLFW action
	if (action == GLFW_PRESS)
	{
		bool test = CEGUI::System::getSingleton().getDefaultGUIContext().injectMouseButtonDown(CEGUIMouseButton);
		return;
	}
	else if (action == GLFW_RELEASE)
	{
		CEGUI::System::getSingleton().getDefaultGUIContext().injectMouseButtonUp(CEGUIMouseButton);
	}


}

//Sets up callbacks
void InitializeInput()
{
	glfwSetKeyCallback(window, KeyboardCallback);
	glfwSetMouseButtonCallback(window, MouseButtonCallback);
}

void UpdateInput(){

	//Global Input Frame Update
	if (selectedObjectPhys != NULL)
	{
		stats->Label->appendText("Object Selected: True\n");
	}
	else
	{
		stats->Label->appendText("Object Selected: False\n");
	}

	shiftHeldDown = false;

	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS)
		shiftHeldDown = true;

	//Update Input Components
	if (glfwGetWindowAttrib(window, GLFW_FOCUSED) && !console->editbox->isActive())
	{
		for (int i = 0; i < InputList.size(); i++)
		{
			InputList[i]->Update();
		}
	}	

	//CEGUI Mouse Position Injection 
	double xpos, ypos;
	glfwGetCursorPos(window, &xpos, &ypos);
	CEGUI::System::getSingleton().getDefaultGUIContext().injectMousePosition(xpos, ypos);
	return;
}

//Converts mouse location to a ray casted from that point through camera 
void ScreenPosToWorldRay(
	int mouseX, int mouseY,             // Mouse position, in pixels, from bottom-left corner of the window
	int screenWidth, int screenHeight,  // Window size, in pixels
	glm::mat4 ViewMatrix,               // Camera position and orientation
	glm::mat4 ProjectionMatrix,         // Camera parameters (ratio, field of view, near and far planes)
	glm::vec3& out_origin,              // Ouput : Origin of the ray. /!\ Starts at the near plane, so if you want the ray to start at the camera's position instead, ignore this.
	glm::vec3& out_direction            // Ouput : Direction, in world space, of the ray that goes "through" the mouse.
	){

	// The ray Start and End positions, in Normalized Device Coordinates (Have you read Tutorial 4 ?)
	glm::vec4 lRayStart_NDC(
		((float)mouseX / (float)screenWidth - 0.5f) * 2.0f, 
		((float)mouseY / (float)screenHeight - 0.5f) * -2.0f,
		-1.0, // The near plane maps to Z=-1 in Normalized Device Coordinates
		1.0f
		);
	glm::vec4 lRayEnd_NDC(
		((float)mouseX / (float)screenWidth - 0.5f) * 2.0f,
		((float)mouseY / (float)screenHeight - 0.5f) * -2.0f,
		0.0,
		1.0f
		);


	// The Projection matrix goes from Camera Space to NDC.
	// So inverse(ProjectionMatrix) goes from NDC to Camera Space.
	glm::mat4 InverseProjectionMatrix = glm::inverse(ProjectionMatrix);

	// The View Matrix goes from World Space to Camera Space.
	// So inverse(ViewMatrix) goes from Camera Space to World Space.
	glm::mat4 InverseViewMatrix = glm::inverse(ViewMatrix);

	glm::vec4 lRayStart_camera = InverseProjectionMatrix * lRayStart_NDC;    lRayStart_camera /= lRayStart_camera.w;
	glm::vec4 lRayStart_world = InverseViewMatrix       * lRayStart_camera; lRayStart_world /= lRayStart_world.w;
	glm::vec4 lRayEnd_camera = InverseProjectionMatrix * lRayEnd_NDC;      lRayEnd_camera /= lRayEnd_camera.w;
	glm::vec4 lRayEnd_world = InverseViewMatrix       * lRayEnd_camera;   lRayEnd_world /= lRayEnd_world.w;


	// Faster way (just one inverse)
	//glm::mat4 M = glm::inverse(ProjectionMatrix * ViewMatrix);
	//glm::vec4 lRayStart_world = M * lRayStart_NDC; lRayStart_world/=lRayStart_world.w;
	//glm::vec4 lRayEnd_world   = M * lRayEnd_NDC  ; lRayEnd_world  /=lRayEnd_world.w;


	glm::vec3 lRayDir_world(lRayEnd_world - lRayStart_world);
	lRayDir_world = glm::normalize(lRayDir_world);


	out_origin = glm::vec3(lRayStart_world);
	out_direction = glm::normalize(lRayDir_world);
}

//Converts GLFW input to good stuff
CEGUI::MouseButton GlfwToCeguiButton(int glfwButton)
{
	switch (glfwButton)
	{
	case GLFW_MOUSE_BUTTON_LEFT: return CEGUI::LeftButton;
	case GLFW_MOUSE_BUTTON_RIGHT: return CEGUI::RightButton;
	case GLFW_MOUSE_BUTTON_MIDDLE: return CEGUI::MiddleButton;
	default: return CEGUI::NoButton;
	}
}

CEGUI::Key::Scan GlfwToCeguiKey(int glfwKey)
{
	switch (glfwKey)
	{
	case GLFW_KEY_ESCAPE: return CEGUI::Key::Escape;
	case GLFW_KEY_F1: return CEGUI::Key::F1;
	case GLFW_KEY_F2: return CEGUI::Key::F2;
	case GLFW_KEY_F3: return CEGUI::Key::F3;
	case GLFW_KEY_F4: return CEGUI::Key::F4;
	case GLFW_KEY_F5: return CEGUI::Key::F5;
	case GLFW_KEY_F6: return CEGUI::Key::F6;
	case GLFW_KEY_F7: return CEGUI::Key::F7;
	case GLFW_KEY_F8: return CEGUI::Key::F8;
	case GLFW_KEY_F9: return CEGUI::Key::F9;
	case GLFW_KEY_F10: return CEGUI::Key::F10;
	case GLFW_KEY_F11: return CEGUI::Key::F11;
	case GLFW_KEY_F12: return CEGUI::Key::F12;
	case GLFW_KEY_F13: return CEGUI::Key::F13;
	case GLFW_KEY_F14: return CEGUI::Key::F14;
	case GLFW_KEY_F15: return CEGUI::Key::F15;
	case GLFW_KEY_UP: return CEGUI::Key::ArrowUp;
	case GLFW_KEY_DOWN: return CEGUI::Key::ArrowDown;
	case GLFW_KEY_LEFT: return CEGUI::Key::ArrowLeft;
	case GLFW_KEY_RIGHT: return CEGUI::Key::ArrowRight;
	case GLFW_KEY_LEFT_SHIFT: return CEGUI::Key::LeftShift;
	case GLFW_KEY_RIGHT_SHIFT: return CEGUI::Key::RightShift;
	case GLFW_KEY_LEFT_CONTROL: return CEGUI::Key::LeftControl;
	case GLFW_KEY_RIGHT_CONTROL: return CEGUI::Key::RightControl;
	case GLFW_KEY_LEFT_ALT: return CEGUI::Key::LeftAlt;
	case GLFW_KEY_RIGHT_ALT: return CEGUI::Key::RightAlt;
	case GLFW_KEY_TAB: return CEGUI::Key::Tab;
	case GLFW_KEY_ENTER: return CEGUI::Key::Return;
	case GLFW_KEY_BACKSPACE: return CEGUI::Key::Backspace;
	case GLFW_KEY_INSERT: return CEGUI::Key::Insert;
	case GLFW_KEY_DELETE: return CEGUI::Key::Delete;
	case GLFW_KEY_PAGE_UP: return CEGUI::Key::PageUp;
	case GLFW_KEY_PAGE_DOWN: return CEGUI::Key::PageDown;
	case GLFW_KEY_HOME: return CEGUI::Key::Home;
	case GLFW_KEY_END: return CEGUI::Key::End;
	case GLFW_KEY_KP_ENTER: return CEGUI::Key::NumpadEnter;
	case GLFW_KEY_CAPS_LOCK: return CEGUI::Key::Capital;
	default: return CEGUI::Key::Unknown;
	}
}

int charCheck(int glfwKey)
{
	//Shift not held
	if (!shiftHeldDown)
	{
		//Convert normally uppercase alphabetical keys to lower
		switch (glfwKey)
		{
		case 65: return 97;  //a
		case 66: return 98;  //b
		case 67: return 99;  //c
		case 68: return 100; //d
		case 69: return 101; //e
		case 70: return 102; //f
		case 71: return 103; //g
		case 72: return 104; //h
		case 73: return 105; //i
		case 74: return 106; //j
		case 75: return 107; //k
		case 76: return 108; //l
		case 77: return 109; //m
		case 78: return 110; //n
		case 79: return 111; //o
		case 80: return 112; //p
		case 81: return 113; //q
		case 82: return 114; //r
		case 83: return 115; //s
		case 84: return 116; //t
		case 85: return 117; //u
		case 86: return 118; //v
		case 87: return 119; //w
		case 88: return 120; //x
		case 89: return 121; //y
		case 90: return 122; //z

		default:
			break;
		}
	}
	else
	{
		//Shift held, take care of non-alphabetical keys
		switch (glfwKey)
		{
		case 96:return 126;//~
		case 49:return 33; //!
		case 50:return 64; //@
		case 51:return 35; //#
		case 52:return 36; //$
		case 53:return 37; //%
		case 54:return 94; //^
		case 55:return 38; //&
		case 56:return 42; //*
		case 57:return 40; //(
		case 48:return 41; //)
		case 45:return 95; //_
		case 61:return 43; //+
		case 91:return 123;//{
		case 93:return 125;//}
		case 59:return 58; //:
		case 39:return 34; //"
		case 92:return 124;//|
		case 44:return 60; //<
		case 46:return 62; //>
		case 47:return 63; //?
			
		default:
			break;
		}
	}

	//Numpad keys, don't care about shift
	switch (glfwKey)
	{
	case 320:return 48;//0
	case 321:return 49;//1
	case 322:return 50;//2
	case 323:return 51;//3
	case 324:return 52;//4
	case 325:return 53;//5
	case 326:return 54;//6
	case 327:return 55;//7
	case 328:return 56;//8
	case 329:return 57;//9
	case 330:return 46;//.
	case 331:return 47;///
	case 332:return 42;//*
	case 333:return 45;//-
	case 334:return 43;//+

	//Kill these keys regardless, -1 sentinel
	case 281:
	case 282:
	case 283:
	case 284:
	case 343:
	case 347:
	case 348:
		return -1;
	
	default:
		break;
	}

	return glfwKey;
	
}