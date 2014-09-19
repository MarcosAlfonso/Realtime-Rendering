#include "InputComponent.h"

InputComponent::InputComponent(BaseEntity* parent)
{
	parentEntity = parent;
}


InputComponent::~InputComponent()
{

}

void InputComponent::Cleanup()
{
	delete(this);
}

void InputComponent::Update()
{
	
}

void InputComponent::toString()
{

}

