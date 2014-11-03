#version 400


layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;

in vec3 tePosition[3];
in vec4 tePatchDistance[3];
in float teLevel[3];
in int tePatchID[3];
in vec2 teUV[3];

out vec3 gTriDistance;
out vec4 gPatchDistance;
out float gLevel;
flat out int gPatchID;
out vec2 gUV;
out float height;
out vec3 gFacetNormal;

uniform mat4 MVP;
uniform mat4 V;
uniform mat4 M;

void main()
{	
	gUV = teUV[0];

	gLevel = teLevel[0];


	gPatchDistance = tePatchDistance[0];
	gTriDistance = vec3(1, 0, 0);
	gl_Position = gl_in[0].gl_Position;
	EmitVertex();

	gPatchDistance = tePatchDistance[1];
	gTriDistance = vec3(0, 1, 0);
	gl_Position = gl_in[1].gl_Position;
	EmitVertex();

	gPatchDistance = tePatchDistance[2];
	gTriDistance = vec3(0, 0, 1);
	gl_Position = gl_in[2].gl_Position;
	height = gl_in[2].gl_Position.y;
	EmitVertex();

	EndPrimitive();
}