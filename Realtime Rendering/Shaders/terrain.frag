#version 330 core

// Ouput data
out vec3 outColor;

// Color input from .cpp
uniform vec3 inColor;

void main()
{

	// Output color = red 
	outColor = inColor;

}