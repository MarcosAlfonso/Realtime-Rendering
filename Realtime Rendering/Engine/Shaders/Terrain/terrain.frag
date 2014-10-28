#version 400

out vec4 FragColor;

in vec4 gPatchDistance;
in vec3 gTriDistance;


uniform vec3 LightPosition;
uniform vec3 DiffuseMaterial;
uniform vec3 AmbientMaterial;

float amplify(float d, float scale, float offset)
{
    d = scale * d + offset;
    d = clamp(d, 0, 1);
    d = 1 - exp2(-2*d*d);
    return d;
}

void main()
{
    vec3 color = vec3(1,1,1);


        float d1 = min(min(gTriDistance.x, gTriDistance.y), gTriDistance.z);
        float d2 = min(min(min(gPatchDistance.x, gPatchDistance.y), gPatchDistance.z), gPatchDistance.w);
        d1 = 1 - amplify(d1, 50, -0.5);
        d2 = amplify(d2, 50, -0.5);
        color = d2 * color + d1 * d2 * vec3(0,0,0);
    

    FragColor = vec4(color, 1.0);
}