#version 330 core

//ins
layout(location = 0) in vec3 position;

//outs
out vec3 Position_worldspace;

//uniforms
uniform mat4 VP;

void main()
{
	// Output position of the vertex, in clip space : MVP * position
	gl_Position =  VP * vec4(position,1);
}