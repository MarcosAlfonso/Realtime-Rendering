#pragma once
#include "Mesh.h"

#include <glm/glm.hpp>

#include <btBulletCollisionCommon.h>

class GridMesh : public Mesh
{
public:
	GridMesh(int xPoints, int zPoints, float xSpacing, float zSpacing);
	std::vector<float> heightFieldArray;

private:
	int xPoints, zPoints, numPoints;
	float xSpacing, zSpacing;
	void PopulateVertices();
	void PopulateIndices();
	void PopulateNormals();
	void PopulatesUVs();

	int posToCount(int x, int y);
	glm::vec3 getVertex(int i, int j);
};