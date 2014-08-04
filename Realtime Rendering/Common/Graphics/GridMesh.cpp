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

#include "GridMesh.h"
#include "../controls.h"


///////////////////
//Generate a grid//
///////////////////
GridMesh::GridMesh(int _xPoints, int _zPoints, float _xSpacing, float _zSpacing)
{
	xPoints = _xPoints;
	zPoints = _zPoints;
	xSpacing = _xSpacing;
	zSpacing = _zSpacing;
	numPoints = xPoints*zPoints;

	//Set aside space in vectors
	vertices.reserve(numPoints);
	normals.reserve(numPoints);
	uvs.reserve(numPoints);

	PopulateVertices();
	PopulateIndices();
	PopulateNormals();
	PopulatesUVs();
	PopulateBuffers();

}


GridMesh::~GridMesh()
{
}


void GridMesh::PopulateVertices()
{
	float width = xSpacing * (xPoints - 1);
	float height = zSpacing * (zPoints - 1);
	float minX = -width / 2;
	float minY = -height / 2;

	for (int i = 0; i < xPoints; i++)
	{
		for (int j = 0; j < zPoints; j++)
		{
			float x = minX + i*xSpacing;
			float z = minY + j*zSpacing;

			float r = glm::sqrt(x*x + z*z);

			float y = (glm::sin(3.141592*r)) / (3.141592 * r);

			//y = 0;
			vertices.push_back(glm::vec3(x, y, z));
		}
	}
}

void GridMesh::PopulateIndices()
{
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
}

void GridMesh::PopulateNormals()
{
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
}

void GridMesh::PopulatesUVs()
{
	//Top down planar UV's
	for (int i = 0; i < uvs.capacity(); i++)
	{
		int xLoc = i % xPoints;
		int zLoc = i / zPoints;

		uvs.push_back(glm::vec2(xLoc * (1.0 / xPoints), zLoc * (1.0 / zPoints)));
	}
}