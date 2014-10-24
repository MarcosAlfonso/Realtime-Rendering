#pragma once
#include "BaseComponent.h"

#define GLEW_STATIC
#include <gl/glew.h>

#include <glm/glm.hpp>

class Mesh;
class BaseEntity;

//Render Component, attached to a Entity needing Mesh rendering
class RenderComponent: public BaseComponent
{

public:
	RenderComponent(BaseEntity * _parent, Mesh * _mesh, GLuint _shader, GLuint _texture, bool _isTessellated);
	~RenderComponent();

	void Update();

	void calculateLight();

	bool flipCullFace = false;


private:
	Mesh * mesh;
	GLuint shader_ID;
	GLuint texture_ID;
	float lightRotation = 0;
	glm::vec3 lightDirection;
	bool isTessellated;

};

