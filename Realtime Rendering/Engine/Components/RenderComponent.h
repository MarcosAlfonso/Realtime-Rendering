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
	RenderComponent(std::shared_ptr<BaseEntity> parent, std::shared_ptr<Mesh> _mesh, GLuint _shader, GLuint _texture);
	~RenderComponent();

	void Update();

	void calculateLight();

	bool flipCullFace = false;


private:
	std::shared_ptr<Mesh> mesh;
	GLuint shader_ID;
	GLuint texture_ID;
	float lightRotation = 0;
	glm::vec3 lightDirection;
};

