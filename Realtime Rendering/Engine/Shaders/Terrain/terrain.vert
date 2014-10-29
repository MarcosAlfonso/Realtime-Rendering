#version 400


// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec3 vertexPosition_modelspace;
layout(location = 1) in vec2 vertexUV;
layout(location = 2) in vec3 vertexNormal_modelspace;
layout(location = 3) in vec3 barycentric;

// Values that stay constant for the whole mesh.
uniform mat4 MVP;
uniform mat4 V;
uniform mat4 M;
uniform vec3 LightDirection_worldspace;

uniform sampler2D myTextureSampler;

// Output data ; will be interpolated for each fragment.
out vec3 vPosition;
out float vertexDistance;
//out vec2 TexCoord_CS_in;
//out vec3 Normal_CS_in;
//out vec3 barycentricCoords;

void main(){

	float y = texture(myTextureSampler, vertexUV).r + texture(myTextureSampler, vertexUV).g + texture(myTextureSampler, vertexUV).b;

	// Position of the vertex, in worldspace : M * position
	vPosition = vec3(vertexPosition_modelspace.x, y*10, vertexPosition_modelspace.z);

	vertexDistance = length(( V * M * vec4(vertexPosition_modelspace,1)).xyz);
			
}

