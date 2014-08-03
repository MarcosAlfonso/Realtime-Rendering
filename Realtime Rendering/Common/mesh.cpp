#include <vector>
#include <stdio.h>
#include <string>
#include <cstring>

#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

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
	glGenVertexArrays(1, &vaoID);
	glBindVertexArray(vaoID);
}

///////////////////
//Generate a grid//
///////////////////
void Mesh::generateGrid(int xPoints, int zPoints, float xSpacing, float zSpacing)
{
	//Initialized vars
	float width = xSpacing * (xPoints - 1);
	float height = zSpacing * (zPoints - 1);
	float minX = -width / 2;
	float minY = -height / 2;
	int numPoints = xPoints*zPoints;

	//Set aside space in vectors
	vertices.reserve(numPoints);
	normals.reserve(numPoints);
	uvs.reserve(numPoints);

#pragma region Vertex Creation
	for (int i = 0; i < xPoints; i++)
	{
		for (int j = 0; j < zPoints; j++)
		{
			float x = minX + i*xSpacing;
			float z = minY + j*zSpacing;

			float r = glm::sqrt(x*x + z*z);
			
			float y = (glm::sin(3.141592*r))/(3.141592 * r);
						
			//y = 0;
			vertices.push_back(glm::vec3(x, y, z));						
		}
	}
#pragma endregion

#pragma region Index Creation
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
#pragma endregion

#pragma region Normal Vertex Creation
	//Size normal vector, and initialize
	for (int i = 0; i < normals.capacity(); i++)
		normals.push_back(glm::vec3(0, 0, 0));

	//For all the indices, get a face, calculate its normal, and then use that to add to related vertices
	for (int i = 0; i < indices.size() / 3; i++)
	{
		int index1 = indices[i * 3];
		int index2 = indices[i * 3 + 1];
		int index3 = indices[i * 3 + 2];

		//Calculate face normal
		glm::vec3 side1 = vertices[index1] - vertices[index3];
		glm::vec3 side2 = vertices[index1] - vertices[index2];
		glm::vec3 normal = glm::cross(side1, side2);

		normals[index1] += normal;
		normals[index2] += normal;
		normals[index3] += normal;
	}

	//normalize indices
	for (int i = 0; i < normals.size(); i++)
		normals[i] = glm::normalize(normals[i]);
#pragma endregion

#pragma region UV Creation
	//Top down planar UV's
	for (int i = 0; i < uvs.capacity(); i++)
	{
		int xLoc = i % xPoints;
		int zLoc = i / zPoints;

		uvs.push_back(glm::vec2(xLoc * (1.0 / xPoints),zLoc * (1.0 / zPoints)));
 	}
#pragma endregion

#pragma region Buffer Creation
	//Vertex, UVs, Normals buffers
	glGenBuffers(1, &verticesID);
	glBindBuffer(GL_ARRAY_BUFFER, verticesID);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);

	glGenBuffers(1, &uvsID);
	glBindBuffer(GL_ARRAY_BUFFER, uvsID);
	glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(glm::vec2), &uvs[0], GL_STATIC_DRAW);

	glGenBuffers(1, &normalsID);
	glBindBuffer(GL_ARRAY_BUFFER, normalsID);
	glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3), &normals[0], GL_STATIC_DRAW);

	// Generate a buffer for the indices as well
	glGenBuffers(1, &indicesID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indicesID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned short), &indices[0], GL_STATIC_DRAW);
}
#pragma endregion

/////////////////////////////////
//Load imported model into Mesh//
/////////////////////////////////
void Mesh::loadFromFile(const char * path)
{
	//Import mesh function through Assimp, fills arrays
	ImportMeshWithAssimp(path, indices, vertices, uvs, normals);

	//Vertex, UVs, Normals buffers
	glGenBuffers(1, &verticesID);
	glBindBuffer(GL_ARRAY_BUFFER, verticesID);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);

	glGenBuffers(1, &uvsID);
	glBindBuffer(GL_ARRAY_BUFFER, uvsID);
	glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(glm::vec2), &uvs[0], GL_STATIC_DRAW);

	glGenBuffers(1, &normalsID);
	glBindBuffer(GL_ARRAY_BUFFER, normalsID);
	glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3), &normals[0], GL_STATIC_DRAW);

	// Generate a buffer for the indices as well
	glGenBuffers(1, &indicesID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indicesID);
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
