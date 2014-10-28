#version 400


layout(quads) in;
layout(triangle_strip, max_vertices = 4) out;

in vec3 tePosition[4];
in vec3 tePatchDistance[4];

out vec3 gPatchDistance;
out vec2 gTriDistance;

uniform mat4 MVP;
uniform mat4 V;
uniform mat4 M;

void main()
{	
	gPatchDistance = tePatchDistance[0];
	gTriDistance = vec2(0,0);
	gl_Position = gl_in[0].gl_Position; EmitVertex();

	gPatchDistance = tePatchDistance[1];
	gTriDistance = vec2(0, 1);
	gl_Position = gl_in[1].gl_Position; EmitVertex();

	gPatchDistance = tePatchDistance[2];
	gTriDistance = vec2(1, 1);
	gl_Position = gl_in[2].gl_Position; EmitVertex();

	gPatchDistance = tePatchDistance[3];
	gTriDistance = vec2(1, 0);
	gl_Position = gl_in[3].gl_Position; EmitVertex();

	EndPrimitive();
}