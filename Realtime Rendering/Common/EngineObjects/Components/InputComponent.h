#pragma once
#include "BaseComponent.h"

class InputComponent :	public BaseComponent
{
public:
	InputComponent(BaseEntity* parent);
	~InputComponent();

	void Update();
	void toString();
	void Cleanup();


};

