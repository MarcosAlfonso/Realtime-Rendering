#ifndef MESH_HPP
#define MESH_HPP

// Include standard headers
#include <stdio.h>
#include <stdlib.h>
#include <vector>

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Mesh
{
public:
	Mesh();
	~Mesh();
	void loadFromFile(const char * path);
	void generateGrid(int xPoints, int yPoints, float xSpacing, float ySpacing);

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

	bool ImportMeshWithAssimp(
		const char * path,
		std::vector<unsigned short> &indices,
		std::vector<glm::vec3> &vertices,
		std::vector<glm::vec2> &uvs,
		std::vector<glm::vec3> &normals
	);

};

#endif