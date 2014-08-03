#include <vector>
#include <stdio.h>
#include <string>
#include <cstring>

#include <glm/glm.hpp>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

// Include AssImp
#include <assimp/Importer.hpp>      // C++ importer interface
#include <assimp/scene.h>           // Output data structure
#include <assimp/postprocess.h>     // Post processing flags

#include "mesh.hpp"
#include "controls.hpp"
#include "meshInstance.hpp"

MeshInstance::MeshInstance()
{
}

MeshInstance::MeshInstance(Mesh * _mesh, GLuint _shader, GLuint _texture)
{
	ModelMatrix = glm::mat4(1.0f);
	mesh = _mesh;
	shader_ID = _shader;
	texture_ID = _texture;
}

////////////////////////
//Render this instance//
////////////////////////
void MeshInstance::Render()
{
	//Binds this Mesh's VAO
	glBindVertexArray(mesh->vaoID);

	glUseProgram(shader_ID);

	//Light positioning TODO should be abstracted
	GLuint LightID = glGetUniformLocation(shader_ID, "LightPosition_worldspace");
	glm::vec3 lightPos = glm::vec3(0, 3, 4);
	glUniform3f(LightID, lightPos.x, lightPos.y, lightPos.z);

	// Get a handle for our "MVP" uniform
	GLuint MatrixID = glGetUniformLocation(shader_ID, "MVP");
	GLuint ViewMatrixID = glGetUniformLocation(shader_ID, "V");
	GLuint ModelMatrixID = glGetUniformLocation(shader_ID, "M");

	// Compute the MVP matrix
	computeMatricesFromInputs();
	glm::mat4 ProjectionMatrix = getProjectionMatrix();
	glm::mat4 ViewMatrix = getViewMatrix();
	glm::mat4 MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;

	// Get a handle for our "myTextureSampler" uniform
	GLuint TextureUniformID = glGetUniformLocation(shader_ID, "DiffuseTextureSampler");

	// Bind our diffuse texture in Texture Unit 0
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture_ID);
	// Set our "DiffuseTextureSampler" sampler to user Texture Unit 0
	glUniform1i(TextureUniformID, 0);

	// Send our transformation to the currently bound shader, 
	// in the "MVP" uniform
	glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
	glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
	glUniformMatrix4fv(ViewMatrixID, 1, GL_FALSE, &ViewMatrix[0][0]);

	//Fill the buffers
	// 1st attribute buffer : vertices
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, mesh->verticesID);
	glVertexAttribPointer(
		0,                  // attribute
		3,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		0,                  // stride
		(void*)0            // array buffer offset
		);

	// 2nd attribute buffer : UVs
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, mesh->uvsID);
	glVertexAttribPointer(
		1,                                // attribute
		2,                                // size
		GL_FLOAT,                         // type
		GL_FALSE,                         // normalized?
		0,                                // stride
		(void*)0                          // array buffer offset
		);

	// 3rd attribute buffer : normals
	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, mesh->normalsID);
	glVertexAttribPointer(
		2,                                // attribute
		3,                                // size
		GL_FLOAT,                         // type
		GL_FALSE,                         // normalized?
		0,                                // stride
		(void*)0                          // array buffer offset
		);

	// Index buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->indicesID);

	// Draw the triangles !
	glDrawElements(
		GL_TRIANGLES,      // mode
		mesh->indices.size(),    // count
		GL_UNSIGNED_SHORT, // type
		(void*)0           // element array buffer offset
		);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
}

glm::mat4 MeshInstance::getModelMatrix()
{
	return ModelMatrix;
}

void MeshInstance::setPosition(glm::vec3 trans)
{
	ModelMatrix = glm::translate(ModelMatrix, trans);
}

void MeshInstance::setRotation(float angle, glm::vec3 rot)
{
	ModelMatrix = glm::rotate(ModelMatrix, angle, rot);
}

void MeshInstance::setScale(glm::vec3 scale)
{
	ModelMatrix = glm::scale(ModelMatrix, scale);
}