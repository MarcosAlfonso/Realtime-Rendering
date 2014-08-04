#pragma once
#include "Mesh.h"

class GridMesh :
	public Mesh
{
public:

	GridMesh(int xPoints, int zPoints, float xSpacing, float zSpacing);
	~GridMesh();

private:

	int xPoints, zPoints, numPoints;
	float xSpacing, zSpacing;

	void PopulateVertices();
	void PopulateIndices();
	void PopulateNormals();
	void PopulatesUVs();

};

