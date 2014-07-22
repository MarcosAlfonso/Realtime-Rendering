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
	Mesh(const char * path);
	~Mesh();
	GLuint getVAO();
	GLuint getBuffer(short index);
	std::vector<unsigned short> getIndices();

private:

	GLuint VAO_ID;
	const char * modelPath;

	bool ImportMeshWithAssimp(
		const char * path,
		std::vector<unsigned short> &indices,
		std::vector<glm::vec3> &vertices,
		std::vector<glm::vec2> &uvs,
		std::vector<glm::vec3> &normals
	);

	std::vector<unsigned short> indices;
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec2> uvs;
	std::vector<glm::vec3> normals;
	
	std::vector<GLuint> buffers;
	GLuint vertexbuffer;
	GLuint uvbuffer;
	GLuint normalbuffer;
	GLuint elementbuffer;

};

#endif