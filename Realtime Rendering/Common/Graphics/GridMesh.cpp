#include <vector>
#include <stdio.h>
#include <iostream>

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

//Noise lib
#include <noise/noise.h>
#include <noise/noiseutils.h>


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



void GridMesh::PopulateVertices()
{
	float width = xSpacing * (xPoints - 1);
	float length = zSpacing * (zPoints - 1);
	float minX = -width / 2;
	float minY = -length / 2;

	noise::module::Perlin PerlinModule;
	PerlinModule.SetOctaveCount(3);

	utils::NoiseMap heightMap;
	utils::NoiseMapBuilderPlane heightMapBuilder;
	heightMapBuilder.SetSourceModule(PerlinModule);
	heightMapBuilder.SetDestNoiseMap(heightMap);
	heightMapBuilder.SetDestSize(xPoints, zPoints);
	heightMapBuilder.SetBounds(0.0, width/60, 0.0, length/60);
	heightMapBuilder.Build();


	for (int i = 0; i < xPoints; i++)
	{
		for (int j = 0; j < zPoints; j++)
		{
			float x = minX + i*xSpacing;
			float z = minY + j*zSpacing;
			
			
			float y = (heightMap.GetValue(i, j)-.5)*10;

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
		glm::vec3 normal = glm::normalize(glm::cross(glm::normalize(side1), glm::normalize(side2)));
		normals[index1] += normal;
		normals[index2] += normal;
		normals[index3] += normal;
	}

	//normalize indices and invert
	for (int i = 0; i < normals.size(); i++)
	{
		normals[i] = glm::normalize(normals[i]);
		normals[i] = normals[i] * glm::vec3(-1);
	}	
}

void GridMesh::PopulatesUVs()
{
	//Top down planar UV's
	for (int i = 0; i < uvs.capacity(); i++)
	{
		int xLoc = i % xPoints;
		int zLoc = i / zPoints;

		float textureRepeat = 32;

		uvs.push_back(glm::vec2(xLoc * (textureRepeat / xPoints), zLoc * (textureRepeat / zPoints)));
	}
}

//Takes a vector pair i,j and returns the linear count location
glm::vec3 GridMesh::getVertex(int i, int j)
{
	if (i >= 0 && j >= 0 && i < xPoints && j < zPoints)
	{
		return vertices[j + (i * zPoints)];
	}
	else
	{
		return glm::vec3(0, 0, 0);
	}

}