#pragma once
#define GLEW_STATIC
#include <gl/glew.h>

GLuint LoadShader(GLuint type, const char * shader_path);
GLuint CreateShaderProgram(const char * vertex_shader_path, const char * fragment_shader_path, const char * tess_control_path, const char * tess_eval_path, const char * geometry_shader_path);
