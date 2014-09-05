#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "bulletDebugDraw.h"
#include "../controls.h"


extern GLuint debugLineShaderID;

void bulletDebugDraw::drawLineTest(const btVector3& from, const btVector3& to, const btVector3& color)
{
	GLfloat vertices[] = {

		0,0,0,
		0,100,0,
		100, 100, 100

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
		0,                  // attribute
		3,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		0,                  // stride
		(void*)0            // array buffer offset
		);


	// Get a handle for our "MVP" uniform
	GLuint MatrixID = glGetUniformLocation(debugLineShaderID, "VP");

	// Compute the MVP matrix
	glm::mat4 ProjectionMatrix = getProjectionMatrix();
	glm::mat4 ViewMatrix = getViewMatrix();
	//glm::mat4 VP = ProjectionMatrix;;// *ViewMatrix;

	//glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &VP[0][0]);

	glDrawArrays(GL_LINE_LOOP, 0, 3);

	glDisableVertexAttribArray(0);


}