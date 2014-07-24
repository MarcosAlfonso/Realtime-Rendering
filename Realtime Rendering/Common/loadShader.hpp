#ifndef SHADER_HPP
#define SHADER_HPP

GLuint LoadShader(GLuint type, const char * shader_path);
GLuint CreateShaderProgram(const char * vertex_shader_path, const char * fragment_shader_path, const char * geometry_shader_path);

#endif
