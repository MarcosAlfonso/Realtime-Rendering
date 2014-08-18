#include <vector>
#include <stdio.h>
#include <string>
#include <cstring>
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
	PerlinModule.SetOctaveCount(1);

	utils::NoiseMap heightMap;
	utils::NoiseMapBuilderPlane heightMapBuilder;
	heightMapBuilder.SetSourceModule(PerlinModule);
	heightMapBuilder.SetDestNoiseMap(heightMap);
	heightMapBuilder.SetDestSize(xPoints, zPoints);
	heightMapBuilder.SetBounds(0.0, width/6, 0.0, length/6);
	heightMapBuilder.Build();


	for (int i = 0; i < xPoints; i++)
	{
		for (int j = 0; j < zPoints; j++)
		{
			float x = minX + i*xSpacing;
			float z = minY + j*zSpacing;
			
			
			float y = heightMap.GetValue(i, j);

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


	//For all vertices, compare heights of surrounding and get normal
	for (int i = 0; i < xPoints; i++)
	{
		for (int j = 0; j < zPoints; j++)
		{
			
			//Gets heights of surrounding points
			float hLeft = getHeight(i - 1, j);
			float hRight = getHeight(i + 1, j);
			float hDown = getHeight(i, j - 1);
			float hUp = getHeight(i, j + j);

			//Calculates normal from those heights
			glm::vec3 normal;
			normal.z = hRight - hLeft;
			normal.y =2;
			normal.x = hUp - hDown;
			normal = glm::normalize(normal);

			normals.push_back(normal);
		}		
	}
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

//Takes a vector pair i,j and returns the linear count location
int GridMesh::posToCount(int i, int j)
{
	return j + (i * zPoints);
}

float GridMesh::getHeight(int i, int j)
{
	//If valid point in mesh, not outside bounds
	if (i >= 0 && j >= 0 && i < xPoints && j < zPoints)
	{
		return vertices[posToCount(i, j)].y;
	}
	else
	{
		return 0; 
	}
}