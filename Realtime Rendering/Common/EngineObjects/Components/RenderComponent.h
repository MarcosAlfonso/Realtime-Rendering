#pragma once
#include "../../Graphics/mesh.h"

class RenderComponent
{

public:
	RenderComponent();
	RenderComponent(Mesh * _mesh, GLuint _shader, GLuint _texture);
	~RenderComponent();

	void setPosition(glm::vec3 trans);

	void setRotation(glm::vec3 eulers);

	void setScale(glm::vec3 scale);

	void Render();
	void calculateLight();
	void calculateModelMatrix();

private:
	Mesh * mesh;
	GLuint shader_ID;
	GLuint texture_ID;

	//Transformation 
	glm::mat4 ModelMatrix;
	glm::mat4 transMatrix;
	glm::mat4 rotMatrix;
	glm::mat4 scaleMatrix;

};

