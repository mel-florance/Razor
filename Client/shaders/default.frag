#version 330 core

#include lighting.glsl
#include tonemapping.glsl

#define MAX_DIRECTIONAL_LIGHTS 1
#define MAX_POINT_LIGHTS 1
#define MAX_SPOT_LIGHTS 1

out vec4 FragColor;

struct Material
{
	float alpha;
	sampler2D diffuseMap;
	sampler2D specularMap;
	sampler2D normalMap;
	sampler2D shadowMap;
	vec3 diffuseColor;
	vec3 specularColor;
	vec3 ambientColor;
	float shininess;
	float shininess_strength;
	float normal_strength;
	int hasDiffuse;
	int hasSpecular;
	int hasNormal;
	vec2 diffuse_tiling;
	vec2 specular_tiling;
	vec2 normal_tiling;
};

struct Directional
{
	vec3 position;
	vec3 direction;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float intensity;
};

struct Point 
{
	vec3 position;
	float constant;
	float linear;
	float quadratic;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float intensity;
};

struct Spot 
{
	vec3 position;
    vec3 direction;
	float intensity;
    float inner_cutoff;
    float outer_cutoff;
    float constant;
    float linear;
    float quadratic;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

in vec2 f_uvs;
in vec3 f_position;
in vec3 f_normal;
in vec3 f_tangent;
in vec4 f_posLightSpace;
in mat3 f_tbn;
in float f_distance;

uniform Material material;
uniform vec3 viewPosition; 
uniform vec3 lightPos;
uniform float shadow_bias;
uniform float shadow_alpha;
uniform int pcf_samples;

uniform Directional directionalLights[MAX_DIRECTIONAL_LIGHTS];
uniform Point pointLights[MAX_POINT_LIGHTS];
uniform Spot spotLights[MAX_SPOT_LIGHTS];

vec3 CalcDirectionalLight(Directional light, vec3 normal, vec3 viewDir);
vec3 CalcPointLight(Point light, vec3 normal, vec3 fragPos, vec3 viewDir);
vec3 CalcSpotLight(Spot light, vec3 normal, vec3 fragPos, vec3 viewDir);

float CalcShadows(vec4 fragPosLightSpace, Material material, vec3 f_normal, vec2 f_uvs, mat3 f_tbn, vec3 f_position, vec3 lightPos)
{
    vec3 projCoords = fragPosLightSpace.xyz;
    projCoords = projCoords * 0.5 + 0.5;

	if(projCoords.z > 1.0)
		projCoords.z = 1.0;

    float closestDepth = texture(material.shadowMap, projCoords.xy).r; 
    float currentDepth = projCoords.z;
    vec3 normal = normalize(f_normal);

	if(material.hasNormal == 1)
	{
		vec3 normal_texel = mix(f_normal, 2.0f * texture(material.normalMap, vec2(f_uvs.x * material.normal_tiling.x, f_uvs.y * material.normal_tiling.y)).rgb - 1.0f, 0.5);
		normal_texel.xy *= material.normal_strength;
		normal_texel = normalize(normal_texel);
		normal = normalize(f_tbn * normal_texel);
	}

    vec3 lightDir = normalize(lightPos - f_position);
    float bias = max(shadow_bias * (1.0 - dot(normal, lightDir)), shadow_bias);

    float shadow = 0.0;
    vec2 texelSize = 1.0 / textureSize(material.shadowMap, 0);

	int samples = pcf_samples;
	int sample_start = samples % 2 == 0 ? samples + 1 : samples;
	int samples_squared = samples * samples;

    for(int y = -sample_start; y <= sample_start; y++)
    {
        for(int x = -sample_start; x <= sample_start; x++)  
        {
            float pcfDepth = texture2D(material.shadowMap, projCoords.xy + vec2(x, y) * texelSize).r; 
            shadow += (currentDepth - bias) > pcfDepth  ? shadow_alpha : 0.0;
        }    
    }
  
	shadow /= samples_squared;

    return shadow;
}

vec3 applyFog( in vec3  rgb,      // original color of the pixel
               in float dist, // camera to point distance
               in vec3  rayOri,   // camera position
               in vec3  rayDir)  // camera to point vector
{
	float c = 0.002f;
	float b = 0.07;
    float fogAmount = c * exp(-rayOri.y * b) * (1.0-exp(-dist * rayDir.y * b )) / rayDir.y;
    vec3 fogColor = vec3(0.5, 0.6, 0.7);
    return mix(rgb, fogColor, fogAmount);
}

void main() 
{
	vec3 norm = normalize(f_normal);
    vec3 viewDir = normalize(viewPosition - f_position);
	vec3 result = vec3(0.0f);

	for(int i = 0; i < MAX_DIRECTIONAL_LIGHTS - 1; i++)
		result += CalcDirectionalLight(directionalLights[i], norm, viewDir);

	for(int i = 0; i < MAX_POINT_LIGHTS - 1; i++)
		result += CalcPointLight(pointLights[i], norm, f_position, viewDir);

	for(int i = 0; i < MAX_SPOT_LIGHTS - 1; i++)
		result += CalcSpotLight(spotLights[i], norm, f_position, viewDir); 
		
//	result += applyFog(result, f_distance, viewPosition, viewDir);

    FragColor = vec4(result, material.alpha);
    //FragColor.rgb = f_tangent;
	//FragColor.rgb = tonemapUncharted2(FragColor.rgb);
}

vec3 CalcDirectionalLight(Directional light, vec3 normal, vec3 viewDir)
{
	vec3 lighting = vec3(0.0); 
	
	if(material.hasNormal == 1)
	{
		vec3 normal_texel = mix(normal, 2.0f * texture(material.normalMap, vec2(f_uvs.x * material.normal_tiling.x, f_uvs.y * material.normal_tiling.y)).rgb - 1.0f, 0.5f);
		normal_texel.xy *= material.normal_strength;
		normal_texel = normalize(normal_texel);
		normal = normalize(f_tbn * normal_texel);
	}

	vec3 lightDir = normalize(light.position - light.direction);
	float dotL = dot(normal, lightDir);
    float diff = max(0.0, dotL) * light.intensity;

	vec3 diffuse = vec3(0.0f);
	vec3 specular = vec3(0.0f);
	vec3 ambient = light.ambient * material.ambientColor;

	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0f), material.shininess) * material.shininess_strength;

	if(material.hasDiffuse == 1)
	{
		vec4 texel = texture(material.diffuseMap, vec2(f_uvs.x * material.diffuse_tiling.x, f_uvs.y * material.diffuse_tiling.y));

		if(texel.a <= 0.0f)
			discard;

		diffuse = light.diffuse * material.diffuseColor * diff * vec3(texel);
		ambient = light.ambient * material.ambientColor * vec3(texture(material.diffuseMap, vec2(f_uvs.x * material.diffuse_tiling.x, f_uvs.y * material.diffuse_tiling.y)));
	}
	else {
		ambient = light.ambient * material.ambientColor;
		diffuse = light.diffuse * diff * material.diffuseColor;
	}
	
	if(material.hasSpecular == 1)
		specular = light.specular * material.specularColor * spec * vec3(texture(material.specularMap, vec2(f_uvs.x * material.specular_tiling.x, f_uvs.y * material.specular_tiling.y)));
	else
		specular = light.specular * spec * material.specularColor;

	float shadow = CalcShadows(f_posLightSpace, material, f_normal, f_uvs, f_tbn, f_position, lightPos);
	lighting = ambient + (1.0 - ambient - shadow) * (diffuse + specular);

    return lighting;
}

vec3 CalcPointLight(Point light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
	if(material.hasNormal == 1)
	{
		vec3 normal_texel = mix(normal, 2.0f * texture(material.normalMap, vec2(f_uvs.x * material.normal_tiling.x, f_uvs.y * material.normal_tiling.y)).rgb - 1.0f, 0.5);
		normal_texel.xy *= material.normal_strength;
		normal_texel = normalize(normal_texel);
		normal = normalize(f_tbn * normal_texel);
	}

    vec3 lightDir = normalize(light.position - fragPos);
    float diff = max(dot(normal, lightDir), 0.0f) * light.intensity;

    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0f), material.shininess);

    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    

	vec3 diffuse = vec3(0.0f);
	vec3 specular = vec3(0.0f);

    vec3 ambient = light.ambient * material.ambientColor * vec3(texture(material.diffuseMap, vec2(f_uvs.x * material.diffuse_tiling.x, f_uvs.y * material.diffuse_tiling.y)));
    
	if(material.hasDiffuse == 1)
		diffuse = light.diffuse * material.diffuseColor * diff * vec3(texture(material.diffuseMap, vec2(f_uvs.x * material.diffuse_tiling.x, f_uvs.y * material.diffuse_tiling.y)));
	else
		diffuse = light.diffuse * diff * material.diffuseColor;

	if(material.hasSpecular == 1)
		specular = light.specular * material.specularColor * spec * vec3(texture(material.specularMap, vec2(f_uvs.x * material.specular_tiling.x, f_uvs.y * material.specular_tiling.y)));
	else
		specular = light.specular * spec * material.specularColor;

    ambient  *= attenuation;
    diffuse  *= attenuation;
    specular *= attenuation;

    return (ambient + diffuse + specular);
}

vec3 CalcSpotLight(Spot light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
	if(material.hasNormal == 1)
	{
		vec3 normal_texel = mix(normal, 2.0f * texture(material.normalMap,  vec2(f_uvs.x * material.normal_tiling.x, f_uvs.y * material.normal_tiling.y)).rgb - 1.0f, 0.5);
		normal_texel.xy *= material.normal_strength;
		normal_texel = normalize(normal_texel);
		normal = normalize(f_tbn * normal_texel);
	}		

	vec3 lightDir = normalize(light.position - fragPos);
    float diff = max(dot(normal, lightDir), 0.0f) * light.intensity;

    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0f), material.shininess);

    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    

    float theta     = dot(lightDir, normalize(-light.direction)); 
    float epsilon   = light.inner_cutoff - light.outer_cutoff;
    float intensity = clamp((theta - light.outer_cutoff) / epsilon, 0.0f, 1.0f);

    vec3 ambient = light.ambient * material.ambientColor * vec3(texture(material.diffuseMap, vec2(f_uvs.x * material.diffuse_tiling.x, f_uvs.y * material.diffuse_tiling.y)));
	
	vec3 diffuse = vec3(0.0f);
	vec3 specular = vec3(0.0f);

	if(material.hasDiffuse == 1)
		diffuse = light.diffuse * material.diffuseColor * diff * vec3(texture(material.diffuseMap, vec2(f_uvs.x * material.diffuse_tiling.x, f_uvs.y * material.diffuse_tiling.y)));
	else
		diffuse = light.diffuse * diff * material.diffuseColor;

	if(material.hasSpecular == 1)
		specular = light.specular * material.specularColor * spec * vec3(texture(material.specularMap, vec2(f_uvs.x * material.specular_tiling.x, f_uvs.y * material.specular_tiling.y)));
	else
		specular = light.specular * spec * material.specularColor;

    ambient  *= attenuation * intensity;
    diffuse  *= attenuation * intensity;
    specular *= attenuation * intensity;

    return (ambient + diffuse + specular);
}
