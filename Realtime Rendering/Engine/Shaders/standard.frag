#version 330 core

// Interpolated values from the vertex shaders
in vec2 UV;
in vec3 Position_worldspace;
in vec3 Normal_worldspace;
in vec3 barycentricCoords;
in float vertexDistance;


// Ouput data
out vec3 color;

// Values that stay constant for the whole mesh.
uniform sampler2D myTextureSampler;
uniform mat4 MV;
uniform vec3 LightDirection_worldspace;
uniform bool isSelected;

#extension GL_OES_standard_derivatives : enable
float edgeFactor(){
	vec3 d = fwidth(barycentricCoords);
	vec3 a3 = smoothstep(vec3(0.0), d*1.5, barycentricCoords);
	return min(min(a3.x, a3.y), a3.z);
}

void main(){

	// Light emission properties
	// You probably want to put them as uniforms
	vec3 LightColor = vec3(.97,.97, 1);
	float LightPower = 0.8f;
	
	// Material properties
	vec3 MaterialDiffuseColor = texture2D( myTextureSampler, UV ).rgb;
	vec3 MaterialAmbientColor = vec3(0.3,0.3,0.4) * MaterialDiffuseColor;

	//Normalize face normal
	vec3 n = normalize( Normal_worldspace );
	// Direction of the light (from the fragment to the light)
	vec3 l = normalize( LightDirection_worldspace );
	// Cosine of the angle between the normal and the light direction, 
	// clamped above 0
	//  - light is at the vertical of the triangle -> 1
	//  - light is perpendicular to the triangle -> 0
	//  - light is behind the triangle -> 0
	float cosTheta = clamp(dot( n,l ), 0,1 );
	
			
	vec3 textureColor = 
		// Ambient : simulates indirect lighting
		MaterialAmbientColor +
		// Diffuse : "color" of the object
		(MaterialDiffuseColor * LightColor * LightPower * cosTheta);

	
		
	if (isSelected)
		color = mix(vec3(0.0),textureColor, edgeFactor());
	else
		//Fog mix
		color = mix(textureColor, vec3(.65,.75,.77), clamp(vertexDistance/350, 0, 1));
	

}