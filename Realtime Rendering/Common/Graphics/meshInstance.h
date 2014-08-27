#pragma once
#include "mesh.h"

class MeshInstance
{

public:
	MeshInstance();
	MeshInstance(Mesh * _mesh, GLuint _shader, GLuint _texture);
	~MeshInstance();

	void setPosition(glm::vec3 trans);
	void addPosition(glm::vec3 trans);

	void setRotation(glm::vec3 eulers);
	void addRotation(glm::vec3 eulers);

	void setScale(glm::vec3 scale);
	void addScale(glm::vec3 scale);

	void Render();
	void RenderGrid();
	void calculateLight();

private:
	Mesh * mesh;
	GLuint shader_ID;
	GLuint texture_ID;
	
	//Transformation 
	glm::mat4 ModelMatrix;
	glm::vec3 transVec = glm::vec3(0);
	glm::vec3 rotVec = glm::vec3(0);
	glm::vec3 scaleVec = glm::vec3(0);
};

