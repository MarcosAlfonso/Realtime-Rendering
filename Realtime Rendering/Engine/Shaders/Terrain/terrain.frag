#version 400

out vec4 FragColor;

in vec4 gPatchDistance;
in vec3 gTriDistance;
in vec3 gFacetNormal;
in float gLevel;
flat in int gPatchID;
in vec2 gUV;
in float height;

uniform int wireframe; //bool
uniform int colorcode; //bool
uniform vec3 LightPosition;
uniform vec3 DiffuseMaterial;
uniform vec3 AmbientMaterial;

uniform sampler2D myTextureSampler;


float amplify(float d, float scale, float offset)
{
	d = scale * d + offset;
	d = clamp(d, 0, 1);
	d = 1 - exp2(-2*d*d);
	return d;
}

void main()
{
	vec3 color = vec3(1,1,1);//mix(vec3(1,1,1), vec3(0,0,0), height/100.0);
	
	if (colorcode == 1)
	{
		color = mix(vec3(1,0,0),vec3(.23,.47,.14),(gLevel/24));
	}

	if (wireframe == 1)
	{
		float d1 = min(min(gTriDistance.x, gTriDistance.y), gTriDistance.z);
		float d2 = min(min(min(gPatchDistance.x, gPatchDistance.y), gPatchDistance.z), gPatchDistance.w);
		d1 = 1 - amplify(d1, 20, -0.5);
		d2 = amplify(d2, 50, -0.5);
		color = d2 * color + d1 * d2 * vec3(1,1,1);
	}

	FragColor = vec4(color, 1.0);

}