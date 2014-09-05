#version 150

//ins
layout(location = 0) in vec3 position;

//outs
out vec3 Position_worldspace;

//uniforms
uniform mat4 VP;

void main()
{
    gl_Position = vec4(position, 1.0);
}