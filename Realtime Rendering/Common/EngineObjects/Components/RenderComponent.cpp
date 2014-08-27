#include <vector>
#include <stdio.h>
#include <string>
#include <cstring>


#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>




#include <GL/glew.h>
#include <GLFW/glfw3.h>

// Include AssImp
#include <assimp/Importer.hpp>      // C++ importer interface
#include <assimp/scene.h>           // Output data structure
#include <assimp/postprocess.h>     // Post processing flags

#include "mesh.h"
#include "../controls.h"
#include "meshInstance.h"

extern int vertexCount;
float lightRot = 0;
glm::vec3 lightDir;

MeshInstance::MeshInstance()
{
}

MeshInstance::~MeshInstance()
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
	GLuint LightID = glGetUniformLocation(shader_ID, "LightDirection_worldspace");
	calculateLight();
	glUniform3f(LightID, lightDir.x, lightDir.y, lightDir.z);

	// Get a handle for our "MVP" uniform
	GLuint MatrixID = glGetUniformLocation(shader_ID, "MVP");
	GLuint ViewMatrixID = glGetUniformLocation(shader_ID, "V");
	GLuint ModelMatrixID = glGetUniformLocation(shader_ID, "M");

	// Compute the MVP matrix
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

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

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

	vertexCount += mesh->vertices.size();

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

void MeshInstance::setPosition(glm::vec3 trans)
{
	transVec = trans;
	transMatrix = glm::mat4();
	transMatrix = glm::translate(transMatrix, trans);
	calculateModelMatrix();
}



void MeshInstance::setRotation(glm::vec3 eulers)
{
	rotVec = eulers;

	glm::quat tempQuat = glm::quat(eulers);
	float angle = glm::angle(tempQuat);
	glm::vec3 axis = glm::axis(tempQuat);

	rotMatrix = glm::mat4();
	rotMatrix = glm::rotate(transMatrix, angle, axis);
	calculateModelMatrix();

}


void MeshInstance::setScale(glm::vec3 scale)
{
	scaleVec = scale;
	scaleMatrix = glm::mat4();
	scaleMatrix = glm::scale(transMatrix, scale);
	calculateModelMatrix();
}


void MeshInstance::calculateModelMatrix()
{
	ModelMatrix = transMatrix * rotMatrix * scaleMatrix;
}

void MeshInstance::calculateLight()
{
	lightDir = glm::vec3(glm::sin(lightRot), .5, glm::cos(lightRot));
}