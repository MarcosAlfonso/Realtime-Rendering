#version 330 core

// Interpolated values from the vertex shaders
in vec3 vertexPosModelspace;

// Ouput data
out vec3 color;

uniform vec3 groundColor;// = vec3(.95,.9,.8);
uniform vec3 skyColor;// = vec3(.34,.6,.75);

void main(){

	//Based off y position of model, return a value between 0 and 1
	float mixPercent = smoothstep(-1,1,vertexPosModelspace.y);
	
	color = mix(groundColor,skyColor,mixPercent);

}