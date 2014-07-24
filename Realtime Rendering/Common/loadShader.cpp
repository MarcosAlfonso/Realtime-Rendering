#include <stdio.h>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <algorithm>
using namespace std;

#include <stdlib.h>
#include <string.h>

#include <GL/glew.h>

#include "loadShader.hpp"

//Generic Shader Load Function
GLuint LoadShader(GLenum type, const char * shader_path)
{
	GLuint shaderID = glCreateShader(type);

	// Read the Vertex Shader code from the file
	std::string ShaderCode;
	std::ifstream ShaderStream(shader_path, std::ios::in);
	if (ShaderStream.is_open()){
		std::string Line = "";
		while (getline(ShaderStream, Line))
			ShaderCode += "\n" + Line;
		ShaderStream.close();
	}
	else{
		printf("Unable to open %s. !\n", shader_path);
		getchar();
		return 0;
	}

	GLint Result = GL_FALSE;
	int InfoLogLength;

	// Compile Shader
	printf("Compiling shader : %s\n", shader_path);
	char const * shaderSourcePointer = ShaderCode.c_str();
	glShaderSource(shaderID, 1, &shaderSourcePointer, NULL);
	glCompileShader(shaderID);

	// Check Shader
	glGetShaderiv(shaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0){
		std::vector<char> shaderErrorMessage(InfoLogLength + 1);
		glGetShaderInfoLog(shaderID, InfoLogLength, NULL, &shaderErrorMessage[0]);
		printf("%s\n", &shaderErrorMessage[0]);
	}

	return shaderID;

}

//Takes 3 paths to shaders defined in text files: Vertex, Fragment, Geometry. Unneeded shaders should be set to NULL
GLuint CreateShaderProgram(const char * vertex_file_path, const char * fragment_file_path, const char * geometry_file_path){

	GLuint ProgramID = glCreateProgram();
	
	if (vertex_file_path != NULL)
	{
		GLuint VertexShaderID = LoadShader(GL_VERTEX_SHADER, vertex_file_path);
		glAttachShader(ProgramID, VertexShaderID);
		glDeleteShader(VertexShaderID);
	}

	if (fragment_file_path != NULL)
	{
		GLuint FragmentShaderID = LoadShader(GL_FRAGMENT_SHADER, fragment_file_path);
		glAttachShader(ProgramID, FragmentShaderID);
		glDeleteShader(FragmentShaderID);

	}

	if (geometry_file_path != NULL)
	{
		GLuint GeometryShaderID = LoadShader(GL_GEOMETRY_SHADER, geometry_file_path);
		glAttachShader(ProgramID, GeometryShaderID);
		glDeleteShader(GeometryShaderID);
	}

	printf("Linking program\n");
	glLinkProgram(ProgramID);

	GLint Result = GL_FALSE;
	int InfoLogLength;

	// Check the program
	glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
	glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0){
		std::vector<char> ProgramErrorMessage(InfoLogLength + 1);
		glGetProgramInfoLog(ProgramID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
		printf("%s\n", &ProgramErrorMessage[0]);
	}

	return ProgramID;
}


