#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "bulletDebugDraw.h"
#include "../controls.h"
#include <stdexcept>


extern GLuint debugLineShaderID;

void bulletDebugDraw::drawLine(const btVector3& from, const btVector3& to, const btVector3& color)
{


	GLfloat x1 = from.getX();
	GLfloat y1 = from.getY();
	GLfloat z1 = from.getZ();
	GLfloat x2 = to.getX();
	GLfloat y2 = to.getY();
	GLfloat z2 = to.getZ();
	GLfloat vertices[] = {
		x1, y1, z1,
		x2, y2, z2
	};
	GLuint vao;
	glUseProgram(debugLineShaderID);
	//Create VAO for Mesh
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	GLuint verticesVbo;
	glGenBuffers(1, &verticesVbo);
	glBindBuffer(GL_ARRAY_BUFFER, verticesVbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(
		0, // attribute
		3, // size
		GL_FLOAT, // type
		GL_FALSE, // normalized?
		0, // stride
		(void*)0 // array buffer offset
		);
	// Get a handle for our "MVP" uniform
	GLuint MatrixID = glGetUniformLocation(debugLineShaderID, "VP");
	// Compute the MVP matrix
	glm::mat4 ProjectionMatrix = getProjectionMatrix();
	glm::mat4 ViewMatrix = getViewMatrix();
	glm::mat4 VP = ProjectionMatrix*ViewMatrix;
	glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &VP[0][0]);
	glDrawArrays(GL_LINES, 0, 3);
	glDisableVertexAttribArray(0);

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

