#ifndef MESHINST_HPP
#define MESHINST_HPP

// Include standard headers
#include <stdio.h>
#include <stdlib.h>
#include <vector>

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class MeshInstance
{

public:
	MeshInstance();
	MeshInstance(Mesh * _mesh, GLuint _shader, GLuint _texture);
	~MeshInstance();

	glm::mat4 getModelMatrix();
	void setPosition(glm::vec3 trans);
	void setRotation(float angle, glm::vec3 axis);
	void setScale(glm::vec3 scale);

	void Render();

private:
	Mesh * mesh;
	GLuint shader_ID;
	GLuint texture_ID;
	glm::mat4 ModelMatrix;
};

#endif

