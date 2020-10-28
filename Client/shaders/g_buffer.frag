#version 330 core

layout (location = 0) out vec3 gPosition;
layout (location = 1) out vec3 gNormal;
layout (location = 2) out vec4 gAlbedoSpec;

in vec2 TexCoords;
in vec3 FragPos;
in vec3 Normal;
//
//uniform sampler2D texture_diffuse1;
//uniform sampler2D texture_specular1;

struct Material
{
//	float alpha;
	sampler2D diffuseMap;
//	sampler2D specularMap;
//	sampler2D normalMap;
//	sampler2D shadowMap;
	vec3 diffuseColor;
//	vec3 specularColor;
//	vec3 ambientColor;
	float shininess;
//	float shininess_strength;
//	float normal_strength;
	int hasDiffuse;
//	int hasSpecular;
//	int hasNormal;
//	vec2 diffuse_tiling;
//	vec2 specular_tiling;
//	vec2 normal_tiling;
};

uniform Material material;

void main()
{    
    // store the fragment position vector in the first gbuffer texture
    gPosition = FragPos;
    // also store the per-fragment normals into the gbuffer
    gNormal = normalize(Normal);
    // and the diffuse per-fragment color

//    gAlbedoSpec.rgb = texture(texture_diffuse1, TexCoords).rgb;
    // store specular intensity in gAlbedoSpec's alpha component
//    gAlbedoSpec.a = texture(texture_specular1, TexCoords).r;

	if (material.hasDiffuse == 1)
		gAlbedoSpec.rgb = texture(material.diffuseMap, TexCoords).rgb;
	else
		gAlbedoSpec.rgb = material.diffuseColor;
	
	gAlbedoSpec.a = material.shininess;
}