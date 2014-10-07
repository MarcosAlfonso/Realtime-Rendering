#pragma once
#define GLEW_STATIC
#include <gl/glew.h>

#include <glm/glm.hpp>

#include <vector>

//Mesh Class, loads in meshes from OBJ's at the moment
class Mesh
{
public:
	Mesh();
	~Mesh();
	
	bool loadFromFile(const char * path);
	void PopulateBuffers();

	std::vector<unsigned short> indices;
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec2> uvs;
	std::vector<glm::vec3> normals;

	GLuint vaoID;
	GLuint verticesID;
	GLuint uvsID;
	GLuint normalsID;
	GLuint indicesID;

private:

	const char * modelPath;

};