#pragma once
#define GLEW_STATIC
#include <gl/glew.h>

#include <glm/glm.hpp>

#include <btBulletCollisionCommon.h>
#include <btBulletDynamicsCommon.h>

#include <vector>

class bulletDebugDraw : public btIDebugDraw
{
public:
	
	bulletDebugDraw();
	~bulletDebugDraw();

	std::vector<GLfloat> verticesVector;
	int count = 0;

	void drawLine(const btVector3& from, const btVector3& to, const btVector3& color);

	virtual void drawContactPoint(const btVector3& PointOnB, const btVector3& normalOnB, btScalar distance, int lifeTime, const btVector3& color);

	virtual void reportErrorWarning(const char* warningString);

	virtual void draw3dText(const btVector3& location, const char* textString);

	virtual void setDebugMode(int debugMode);

	virtual int getDebugMode() const;

	void DrawLineArray();
private:

};