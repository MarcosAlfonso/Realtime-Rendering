#pragma once
#include "BaseComponent.h"

class GLFWwindow;


class InputComponent :	public BaseComponent
{
public:
	InputComponent(BaseEntity* parent);
	~InputComponent();

	void Update();
	void toString();
	void Cleanup();

	void(*KeyboardInputFunc)(GLFWwindow* window, int key, int scancode, int action, int mods);
	void(*MouseInputFunc)(GLFWwindow* window, int button, int action, int mods);

};

