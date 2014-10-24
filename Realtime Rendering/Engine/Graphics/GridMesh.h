#pragma once
#include "Mesh.h"

#include <glm/glm.hpp>

#include <btBulletCollisionCommon.h>

//Generates a gridded plane mesh of uniformly spaced points
class GridMesh : public Mesh
{
public:
	GridMesh(int _xPoints, int _zPoints, float _xSpacing, float _zSpacing, bool _perlin);
	std::vector<float> heightFieldArray;

	bool isPerlin;

private:
	int xPoints, zPoints, numPoints;
	float xSpacing, zSpacing;
	void PopulateVertices();
	void PopulateIndices();
	void PopulateNormals();
	void PopulatesUVs();
	void PopulateBarycentrics();


	int posToCount(int x, int y);
	glm::vec3 getVertex(int i, int j);
};