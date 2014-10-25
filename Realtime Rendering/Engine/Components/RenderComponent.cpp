#pragma once
#include "RenderComponent.h"

#include "../Graphics/mesh.h"
#include "../InputManager.h"
#include "TransformComponent.h"
#include "../Entities/BaseEntity.h"
#include "CameraComponent.h"


#define GLEW_STATIC
#include <GL/glew.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>

#include <vector>
#include <stdio.h>
#include <string>
#include <cstring>
#include "PhysicsComponent.h"
#include "../GUI/Stats.h"

extern int vertexCount;
extern BaseEntity* mainCamera;

glm::vec3 groundColor = glm::vec3(.95, .9, .8);
glm::vec3 skyColor = glm::vec3(.34, .6, .75);

extern PhysicsComponent * selectedObjectPhys;

int innerTessLevel = 1;
int outerTessLevel = 1;
int tessOverride = 0;

extern char debugBuffer[];
extern Stats * stats;



//Render Component, attached to a Entity needing Mesh rendering
RenderComponent::RenderComponent(BaseEntity * _parent, Mesh * _mesh, GLuint _shader, GLuint _texture, bool _isTessellated)
{
	Name = "Render Component";
	parentEntity = _parent;
	componentType = RENDER;

	mesh = _mesh;
	shader_ID = _shader;
	texture_ID = _texture;

	isTessellated = _isTessellated;

}

RenderComponent::~RenderComponent()
{

}


////////////////////////
//Render this instance//
////////////////////////
void RenderComponent::Update()
{
	//Binds this Mesh's VAO
	glBindVertexArray(mesh->vaoID);

	//Set the shader
	glUseProgram(shader_ID);

	//Light positioning TODO should be abstracted
	GLuint LightID = glGetUniformLocation(shader_ID, "LightDirection_worldspace");
	calculateLight();
	glUniform3f(LightID, lightDirection.x, lightDirection.y, lightDirection.z);

	// Get a handle for our "MVP" uniform
	GLuint MatrixID = glGetUniformLocation(shader_ID, "MVP");
	GLuint ViewMatrixID = glGetUniformLocation(shader_ID, "V");
	GLuint ModelMatrixID = glGetUniformLocation(shader_ID, "M");

	CameraComponent * cam = mainCamera->getElementOfType<CameraComponent>();

	// Compute the MVP matrix
	glm::mat4 ProjectionMatrix = cam->ProjectionMatrix;
	glm::mat4 ViewMatrix = cam->ViewMatrix;
	glm::mat4 MVP = ProjectionMatrix * ViewMatrix * parentEntity->Transform->ModelMatrix;

	// Get a handle for our "myTextureSampler" uniform
	GLuint TextureUniformID = glGetUniformLocation(shader_ID, "DiffuseTextureSampler");


	//SkySphere uniforms
	GLuint groundColorID = glGetUniformLocation(shader_ID, "groundColor");
	GLuint skyColorID = glGetUniformLocation(shader_ID, "skyColor");

	glUniform3f(groundColorID, groundColor.x, groundColor.y, groundColor.z);
	glUniform3f(skyColorID, skyColor.x, skyColor.y, skyColor.z);

	PhysicsComponent * phys = parentEntity->getElementOfType<PhysicsComponent>();

	glUniform1i(glGetUniformLocation(shader_ID, "isSelected"), (phys != nullptr && phys == selectedObjectPhys));

	// Bind our diffuse texture in Texture Unit 0
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture_ID);
	// Set our "DiffuseTextureSampler" sampler to user Texture Unit 0
	glUniform1i(TextureUniformID, 0);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	//Send tessellation levels
	glUniform1i(glGetUniformLocation(shader_ID, "innerTess"), innerTessLevel);
	glUniform1i(glGetUniformLocation(shader_ID, "outerTess"), outerTessLevel);
	glUniform1i(glGetUniformLocation(shader_ID, "tessOverride"), tessOverride);

	//Print tessellation levels
	if (isTessellated)
	{
		sprintf(debugBuffer, "Tess Overriden?: %d\n", tessOverride);
		stats->Label->appendText(debugBuffer);

		sprintf(debugBuffer, "Inner Tess Override: %d\n", innerTessLevel);
		stats->Label->appendText(debugBuffer);

		sprintf(debugBuffer, "Outer Tess Override: %d\n", outerTessLevel);
		stats->Label->appendText(debugBuffer);
	}

	// Send our transformation to the currently bound shader, 
	// in the "MVP" uniform
	glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
	glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &parentEntity->Transform->ModelMatrix[0][0]);
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


	// 4th attribute buffer : barycentricCoords
	glEnableVertexAttribArray(3);
	glBindBuffer(GL_ARRAY_BUFFER, mesh->barycentricsID);
	glVertexAttribPointer(
		3,                                // attribute
		3,                                // size
		GL_FLOAT,                         // type
		GL_FALSE,                         // normalized?
		0,                                // stride
		(void*)0                          // array buffer offset
		);

	// Index buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->indicesID);

	vertexCount += mesh->vertices.size();


	if (isTessellated)
	{
		//Tess Stuff
		glPatchParameteri(GL_PATCH_VERTICES, 3);
		// Draw the Patches  !
		glDrawElements(GL_PATCHES, mesh->indices.size(), GL_UNSIGNED_SHORT, 0);
	}
	else
	{
		glDrawElements(
			GL_TRIANGLES,      // mode
			mesh->indices.size(),    // count
			GL_UNSIGNED_SHORT, // type
			(void*)0           // element array buffer offset
			);
	}
	

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
	glDisableVertexAttribArray(3);
}

void RenderComponent::calculateLight()
{
	lightDirection = glm::vec3(glm::sin(lightRotation), .5, glm::cos(lightRotation));
}