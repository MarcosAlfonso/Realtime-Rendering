#pragma once
#include "BaseComponent.h"

#define GLEW_STATIC
#include <gl/glew.h>

#include <glm/glm.hpp>

class Mesh;
class BaseEntity;

class RenderComponent: public BaseComponent
{

public:
	RenderComponent(BaseEntity* paren, Mesh * _mesh, GLuint _shader, GLuint _texture);
	~RenderComponent();

	void Update();
	void Cleanup();

	void calculateLight();

	bool flipCullFace = false;


private:
	Mesh * mesh;
	GLuint shader_ID;
	GLuint texture_ID;
	float lightRotation = 0;
	glm::vec3 lightDirection;
};

