#pragma once
#include "framebuffer.h""

#include <GL\glew.h>

extern GLuint FramebufferShaderID;
extern GLuint screenUvMapTexture;

Framebuffer::Framebuffer()
{

	// The framebuffer, which regroups 0, 1, or more textures, and 0 or 1 depth buffer.
	frameBuffer_ID = 0;
	glGenFramebuffers(1, &frameBuffer_ID);
	glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer_ID);

	// The texture we're going to render to
	GLuint renderedTexture;
	glGenTextures(1, &renderedTexture_ID);

	// "Bind" the newly created texture : all future texture functions will modify this texture
	glBindTexture(GL_TEXTURE_2D, renderedTexture_ID);

	// Give an empty image to OpenGL ( the last "0" )
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 1920, 1080, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);

	// Poor filtering. Needed !
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	// The depth buffer
	GLuint depthBuffer_ID;
	glGenRenderbuffers(1, &depthBuffer_ID);
	glBindRenderbuffer(GL_RENDERBUFFER, depthBuffer_ID);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, 1920, 1080);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthBuffer_ID);
	
	// Set "renderedTexture" as our colour attachement #0
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, renderedTexture_ID, 0);

	// Set the list of draw buffers.
	DrawBuffers[1] = { GL_COLOR_ATTACHMENT0 };
	glDrawBuffers(1, DrawBuffers); // "1" is the size of DrawBuffers

	// The fullscreen quad's FBO
	GLuint quad_VertexArrayID;
	glGenVertexArrays(1, &quad_VertexArrayID);
	glBindVertexArray(quad_VertexArrayID);

	static const GLfloat g_quad_vertex_buffer_data[] = {
		-1.0f, -1.0f, 0.0f,
		1.0f, -1.0f, 0.0f,
		-1.0f, 1.0f, 0.0f,
		-1.0f, 1.0f, 0.0f,
		1.0f, -1.0f, 0.0f,
		1.0f, 1.0f, 0.0f,
	};

	glGenBuffers(1, &quad_vertexBuffer_ID);
	glBindBuffer(GL_ARRAY_BUFFER, quad_vertexBuffer_ID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_quad_vertex_buffer_data), g_quad_vertex_buffer_data, GL_STATIC_DRAW);

	// Create and compile our GLSL program from the shaders
	shader_textureLoc_ID = glGetUniformLocation(FramebufferShaderID, "renderedTexture");

}

void Framebuffer::set()
{
	// Render to our framebuffer
	glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer_ID);
	glViewport(0, 0, 1920, 1080); // Render on the whole framebuffer, complete from the lower left corner to the upper right

	// Bind our texture in Texture Unit 0
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, screenUvMapTexture);

	// Set our "renderedTexture" sampler to user Texture Unit 0
	glUniform1i(shader_textureLoc_ID, 0);
}

void Framebuffer::RenderToScreen()
{
	//Render the framebuffer to screen
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, 1920, 1080);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUseProgram(FramebufferShaderID);

	// Bind our texture in Texture Unit 0
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, renderedTexture_ID);
	// Set our "renderedTexture" sampler to user Texture Unit 0
	glUniform1i(shader_textureLoc_ID, 0);

	// 1rst attribute buffer : vertices
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, quad_vertexBuffer_ID);
	glVertexAttribPointer(
		0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
		3,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		0,                  // stride
		(void*)0            // array buffer offset
		);

	// Draw the triangles !
	glDrawArrays(GL_TRIANGLES, 0, 6); // 2*3 indices starting at 0 -> 2 triangles

	glDisableVertexAttribArray(0);

}

Framebuffer::~Framebuffer()
{
}