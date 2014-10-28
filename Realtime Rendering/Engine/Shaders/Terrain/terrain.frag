#version 400

out vec4 FragColor;

in vec3 gPatchDistance;
in vec2 gTriDistance;


uniform vec3 LightPosition;
uniform vec3 DiffuseMaterial;
uniform vec3 AmbientMaterial;

void main()
{


    vec3 baseColor = vec3(1.0, 1.0, 1.0);
    vec3 wireColor = vec3(0.4, 0.1, 0.1);
	

	if (gPatchDistance.x < .01 || gPatchDistance.y < .01)
		 FragColor = vec4(wireColor, 1.0);
	else
		 FragColor = vec4(baseColor, 1.0);

}
