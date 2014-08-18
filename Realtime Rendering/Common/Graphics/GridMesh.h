#pragma once
#include "Mesh.h"

class GridMesh :
	public Mesh
{
public:

	GridMesh(int xPoints, int zPoints, float xSpacing, float zSpacing);

private:

	int xPoints, zPoints, numPoints;
	float xSpacing, zSpacing;

	void PopulateVertices();
	void PopulateIndices();
	void PopulateNormals();
	void PopulatesUVs();

	int posToCount(int x, int y);
	float getHeight(int i, int j);
};

