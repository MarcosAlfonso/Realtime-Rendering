#include <GL\glew.h>

class Framebuffer
{
public:
	Framebuffer();
	~Framebuffer();

	void set();
	void RenderToScreen();

	GLuint frameBuffer_ID;
	GLuint depthBuffer_ID;
	GLuint renderedTexture_ID;
	GLuint Texture_ID;
	GLuint shader_textureLoc_ID;
	GLuint quad_vertexBuffer_ID;

private:
	GLenum DrawBuffers[1];

};