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

// Output data ; will be interpolated for each fragment.
out vec3 vPosition;
out float vertexDistance;
//out vec2 TexCoord_CS_in;
//out vec3 Normal_CS_in;
//out vec3 barycentricCoords;

void main(){
	
	// barycentricsCoords for selection
	//barycentricCoords = barycentric;

	// Position of the vertex, in worldspace : M * position
	vPosition = vertexPosition_modelspace.xyz;

	vertexDistance = length(( V * M * vec4(vertexPosition_modelspace,1)).xyz);

	int i = 3;
			
	// Normal of the the vertex, in camera space
	//Normal_CS_in = ( V * M * vec4(vertexNormal_modelspace,0)).xyz;// Only correct if ModelMatrix does not scale the model ! Use its inverse transpose if not.
	
	// UV of the vertex. No special space for this one.
	//TexCoord_CS_in = vertexUV;
}

