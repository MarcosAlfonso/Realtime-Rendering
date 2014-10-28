#version 400


layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;

in vec3 tePosition[3];
in vec4 tePatchDistance[3];

out vec3 gTriDistance;
out vec4 gPatchDistance;

uniform mat4 MVP;
uniform mat4 V;
uniform mat4 M;

void main()
{
    
    gPatchDistance = tePatchDistance[0];
    gTriDistance = vec3(1, 0, 0);
    gl_Position = gl_in[0].gl_Position; EmitVertex();

    gPatchDistance = tePatchDistance[1];
    gTriDistance = vec3(0, 1, 0);
    gl_Position = gl_in[1].gl_Position; EmitVertex();

    gPatchDistance = tePatchDistance[2];
    gTriDistance = vec3(0, 0, 1);
    gl_Position = gl_in[2].gl_Position; EmitVertex();

    EndPrimitive();
}