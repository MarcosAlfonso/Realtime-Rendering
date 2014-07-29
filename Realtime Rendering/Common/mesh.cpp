#include <vector>
#include <stdio.h>
#include <string>
#include <cstring>

#include <glm/glm.hpp>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

// Include AssImp
#include <assimp/Importer.hpp>      // C++ importer interface
#include <assimp/scene.h>           // Output data structure
#include <assimp/postprocess.h>     // Post processing flags

#include "mesh.hpp"
#include "controls.hpp"

Mesh::~Mesh()
{

}

Mesh::Mesh()
{
	//Create VAO for Mesh
	glGenVertexArrays(1, &VAO_ID);
	glBindVertexArray(VAO_ID);
}

///////////////////
//Generate a grid//
///////////////////
void Mesh::generateGrid(int xPoints, int zPoints, float xSpacing, float zSpacing)
{
	float width = xSpacing * (xPoints - 1);
	float height = zSpacing * (zPoints - 1);
	float minX = -width / 2;
	float minY = -height / 2;

	vertices.reserve(xPoints*zPoints);

	//Calculates vertex positions and adds them to array
	for (int i = 0; i < xPoints; i++)
	{
		for (int j = 0; j < zPoints; j++)
		{
			float x = minX + i*xSpacing;
			float y = 0;
			float z = minY + j*zSpacing;

			vertices.push_back(glm::vec3(x, y, z));						
		}
	}

	int quadCount = (xPoints - 1)*(zPoints - 1);

	for (int i = 0; i < quadCount; i++)
	{
		int k = i + i / (xPoints - 1);
		int a = k;
		int b = k + 1;
		int c = k + 1 + xPoints;
		int d = k + xPoints;

		//Triangle 1
		indices.push_back(a);
		indices.push_back(c);
		indices.push_back(d);

		//Triangle 2
		indices.push_back(a);
		indices.push_back(b);
		indices.push_back(c);
	}

	buffers.resize(2);

	//Fill vertex buffer
	glGenBuffers(1, &buffers[0]);
	glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);

	//Fill indices buffer
	glGenBuffers(1, &buffers[1]);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers[1]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned short), &indices[0], GL_STATIC_DRAW);
}

/////////////////////////////////
//Load imported model into Mesh//
/////////////////////////////////
void Mesh::loadFromFile(const char * path)
{
	//Import mesh function through Assimp, fills arrays
	ImportMeshWithAssimp(path, indices, vertices, uvs, normals);

	buffers.resize(4);

	//Create a bunch of buffers for the stuff
	glGenBuffers(1, &buffers[0]);
	glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);

	glGenBuffers(1, &buffers[1]);
	glBindBuffer(GL_ARRAY_BUFFER, buffers[1]);
	glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(glm::vec2), &uvs[0], GL_STATIC_DRAW);

	glGenBuffers(1, &buffers[2]);
	glBindBuffer(GL_ARRAY_BUFFER, buffers[2]);
	glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3), &normals[0], GL_STATIC_DRAW);

	// Generate a buffer for the indices as well
	glGenBuffers(1, &buffers[3]);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers[3]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned short), &indices[0], GL_STATIC_DRAW);

}

//////////////////////////
//Read model with Assimp//
//////////////////////////
bool Mesh::ImportMeshWithAssimp(
	const char * path,
	std::vector<unsigned short> & indices,
	std::vector<glm::vec3> & vertices,
	std::vector<glm::vec2> & uvs,
	std::vector<glm::vec3> & normals
	){

	Assimp::Importer importer;

	const aiScene* scene = importer.ReadFile(path, aiProcess_JoinIdenticalVertices | aiProcess_SortByPType);
	if (!scene) {
		fprintf(stderr, importer.GetErrorString());
		getchar();
		return false;
	}
	const aiMesh* mesh = scene->mMeshes[0]; // In this simple example code we always use the 1st mesh (in OBJ files there is often only one anyway)

	// Fill vertices positions
	vertices.reserve(mesh->mNumVertices);
	for (unsigned int i = 0; i < mesh->mNumVertices; i++){
		aiVector3D pos = mesh->mVertices[i];
		vertices.push_back(glm::vec3(pos.x, pos.y, pos.z));
	}

	// Fill vertices texture coordinates
	uvs.reserve(mesh->mNumVertices);
	for (unsigned int i = 0; i < mesh->mNumVertices; i++){
		aiVector3D UVW = mesh->mTextureCoords[0][i]; // Assume only 1 set of UV coords; AssImp supports 8 UV sets.
		uvs.push_back(glm::vec2(UVW.x, UVW.y));
	}

	// Fill vertices normals
	normals.reserve(mesh->mNumVertices);
	for (unsigned int i = 0; i < mesh->mNumVertices; i++){
		aiVector3D n = mesh->mNormals[i];
		normals.push_back(glm::vec3(n.x, n.y, n.z));
	}

	// Fill face indices
	indices.reserve(3 * mesh->mNumFaces);
	for (unsigned int i = 0; i < mesh->mNumFaces; i++){
		// Assume the model has only triangles.
		indices.push_back(mesh->mFaces[i].mIndices[0]);
		indices.push_back(mesh->mFaces[i].mIndices[1]);
		indices.push_back(mesh->mFaces[i].mIndices[2]);
	}

}

/////////////////////
//Returns ID of VAO//
/////////////////////
GLuint Mesh::getVAO()
{
	return VAO_ID;
}

///////////////////////////////
//Returns the buffer at index//
///////////////////////////////
GLuint Mesh::getBuffer(short index)
{
	return buffers[index];
}

///////////////////
//Returns indices//
///////////////////
std::vector<unsigned short> Mesh::getIndices()
{
	return indices;
}

////////////////////
//Returns vertices//
////////////////////
std::vector<glm::vec3> Mesh::getVertices()
{
	return vertices;
}
