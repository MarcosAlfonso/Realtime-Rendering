#pragma once
#include "BaseComponent.h"

class GLFWwindow;


class InputComponent :	public BaseComponent
{
public:
	virtual void KeyboardInputCallback(GLFWwindow* window, int key, int scancode, int action, int mods) = 0;

	virtual void MouseInputCallback(GLFWwindow* window, int button, int action, int mods) = 0;

};

