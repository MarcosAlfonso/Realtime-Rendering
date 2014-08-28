#pragma once
#include "../../Graphics/mesh.h"
#include "BaseComponent.h"
#include "../GameObject.h"

class RenderComponent: public BaseComponent
{

public:
	RenderComponent(Mesh * _mesh, GLuint _shader, GLuint _texture);
	~RenderComponent();

	void Update();

	void calculateLight();

private:
	Mesh * mesh;
	GLuint shader_ID;
	GLuint texture_ID;
	float lightRotation = 0;
	glm::vec3 lightDirection;
};

