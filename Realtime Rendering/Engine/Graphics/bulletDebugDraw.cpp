#pragma once
#include "bulletDebugDraw.h"

#include "../Entities/FreeCamera.h"

extern FreeCamera * mainCamera;

#define GLEW_STATIC
#include <GL/glew.h>

void bulletDebugDraw::drawLine(const btVector3& from, const btVector3& to, const btVector3& color)
{


	GLfloat x1 = from.getX();
	GLfloat y1 = from.getY();
	GLfloat z1 = from.getZ();
	GLfloat x2 = to.getX();
	GLfloat y2 = to.getY();
	GLfloat z2 = to.getZ();

	glUseProgram(0);

	glm::mat4 ProjectionMatrix = mainCamera->Camera->ProjectionMatrix;
	glm::mat4 ViewMatrix = mainCamera->Camera->ViewMatrix;

	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf((const GLfloat*)&ProjectionMatrix[0]);
	glMatrixMode(GL_MODELVIEW);
	glm::mat4 MV = ViewMatrix;
	glLoadMatrixf((const GLfloat*)&MV[0]);

	glColor3f(1, 1, 1);
	glBegin(GL_LINES);
		glVertex3f(x1, y1, z1);
		glVertex3f(x2, y2, z2);
	glEnd();

}

bulletDebugDraw::bulletDebugDraw()
{

}

bulletDebugDraw::~bulletDebugDraw()
{

}

void bulletDebugDraw::drawContactPoint(const btVector3& PointOnB, const btVector3& normalOnB, btScalar distance, int lifeTime, const btVector3& color)
{
}

void bulletDebugDraw::reportErrorWarning(const char* warningString)
{
}

void bulletDebugDraw::draw3dText(const btVector3& location, const char* textString)
{
}

void bulletDebugDraw::setDebugMode(int debugMode)
{
}

int bulletDebugDraw::getDebugMode() const
{
	return 1;
}

