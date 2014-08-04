#pragma once

class MeshInstance
{

public:
	MeshInstance();
	MeshInstance(Mesh * _mesh, GLuint _shader, GLuint _texture);
	~MeshInstance();

	void setPosition(glm::vec3 trans);
	void setRotation(float angle, glm::vec3 axis);
	void setScale(glm::vec3 scale);

	void Render();
	void RenderGrid();

private:
	Mesh * mesh;
	GLuint shader_ID;
	GLuint texture_ID;
	glm::mat4 ModelMatrix;
};

