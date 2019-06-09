#version 330 core

#include lighting.glsl
#include tonemapping.glsl

#define MAX_DIRECTIONAL_LIGHTS 1
#define MAX_POINT_LIGHTS 1
#define MAX_SPOT_LIGHTS 1

out vec4 FragColor;

struct Material
{
	sampler2D diffuseMap;
	sampler2D specularMap;
	sampler2D normalMap;
	sampler2D shadowMap;
	sampler2D splatmap;
	sampler2D red_channel_diffuse;
	sampler2D red_channel_specular;
	sampler2D red_channel_normal;
	sampler2D green_channel_diffuse;
	sampler2D green_channel_specular;
	sampler2D green_channel_normal;
	sampler2D blue_channel_diffuse;
	sampler2D blue_channel_specular;
	sampler2D blue_channel_normal;
	vec3 diffuseColor;
	vec3 specularColor;
	vec3 ambientColor;
	float shininess;
	float shininess_strength;
	float normal_strength;
	int hasDiffuse;
	int hasSpecular;
	int hasNormal;
	int hasRedChannelDiffuse;
	int hasRedChannelSpecular;
	int hasRedChannelNormal;
	int hasGreenChannelDiffuse;
	int hasGreenChannelSpecular;
	int hasGreenChannelNormal;
	int hasBlueChannelDiffuse;
	int hasBlueChannelSpecular;
	int hasBlueChannelNormal;
	int hasSplatmap;
	vec2 diffuse_tiling;
	vec2 specular_tiling;
	vec2 normal_tiling;
};

struct Directional
{
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
in vec4 f_posLightSpace;
in mat3 f_tbn;

uniform Material material;
uniform vec3 viewPosition; 
uniform vec3 lightPos;

uniform Directional directionalLights[MAX_DIRECTIONAL_LIGHTS];
uniform Point pointLights[MAX_POINT_LIGHTS];
uniform Spot spotLights[MAX_SPOT_LIGHTS];

vec3 CalcDirectionalLight(Directional light, vec3 normal, vec3 viewDir);
vec3 CalcPointLight(Point light, vec3 normal, vec3 fragPos, vec3 viewDir);
vec3 CalcSpotLight(Spot light, vec3 normal, vec3 fragPos, vec3 viewDir);

vec3 blendTextures(vec4 texture1, float a1, vec4 texture2, float a2)  
{  
    float depth = 0.2;  
    float ma = max(texture1.a + a1, texture2.a + a2) - depth;  
  
    float b1 = max(texture1.a + a1 - ma, 0);  
    float b2 = max(texture2.a + a2 - ma, 0);  
  
    return (texture1.rgb * b1 + texture2.rgb * b2) / (b1 + b2);  
}  

float CalcShadows(vec4 fragPosLightSpace, Material material, vec3 f_normal, vec2 f_uvs, mat3 f_tbn, vec3 f_position, vec3 lightPos)
{
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    projCoords = projCoords * 0.5 + 0.5;

    float closestDepth = texture(material.shadowMap, projCoords.xy).r; 
    float currentDepth = projCoords.z;
  
	vec3 normal = vec3(0.0f);
	if(material.hasNormal == 1)
	{
		vec3 normal_texel = 2.0f * texture(material.normalMap, vec2(f_uvs.x * material.normal_tiling.x, f_uvs.y * material.normal_tiling.y)).rgb - 1.0f;

		normal_texel.xy *= material.normal_strength;
		normal = normalize(normal_texel) * f_tbn;
	}

    vec3 lightDir = normalize(lightPos - f_position);
    float bias = max(0.005 * (1.0 - dot(normal, lightDir)), 0.005);

    float shadow = 0.0;
    vec2 texelSize = 1.0 / textureSize(material.shadowMap, 0);

    for(int x = -1; x <= 1; ++x)
    {
        for(int y = -1; y <= 1; ++y)  
        {
            float pcfDepth = texture(material.shadowMap, projCoords.xy + vec2(x, y) * texelSize).r; 
            shadow += currentDepth - bias > pcfDepth  ? 1.0 : 0.0;
        }    
    }

    shadow /= 9.0;

    if(projCoords.z > 1.0)
        shadow = 0.0;
        
    return shadow;
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
		
    FragColor = vec4(result, 1.0f);
	FragColor.rgb = tonemapUncharted2(FragColor.rgb);
}

vec3 CalcDirectionalLight(Directional light, vec3 normal, vec3 viewDir)
{
	vec3 lighting = vec3(0.0); 
	vec4 splatmap_color = vec4(0.0f);

	if(material.hasSplatmap == 1)
		splatmap_color = texture(material.splatmap, f_uvs);

	vec3 final_normal = vec3(0.0f);
	vec3 base_normal = vec3(0.0f);
	vec3 red_channel_normal = vec3(0.0f);
	vec3 green_channel_normal = vec3(0.0f);
	vec3 blue_channel_normal = vec3(0.0f);

	if(material.hasNormal == 1)
	{
		vec3 normal_texel = 2.0f * texture(material.normalMap, vec2(f_uvs.x * material.normal_tiling.x, f_uvs.y * material.normal_tiling.y)).rgb - 1.0f;

		normal_texel.xy *= material.normal_strength;
		final_normal += normal_texel;
	}

	if(material.hasRedChannelNormal == 1)
	{
		vec3 normal_texel = (2.0f * texture(material.red_channel_normal, vec2(f_uvs.x * material.normal_tiling.x, f_uvs.y * material.normal_tiling.y)).rgb - 1.0f) * splatmap_color.r;

		normal_texel.xy *= material.normal_strength;
		final_normal += normal_texel;
	}
	
	if(material.hasGreenChannelNormal == 1)
	{
		vec3 normal_texel = (2.0f * texture(material.green_channel_normal, vec2(f_uvs.x * material.normal_tiling.x, f_uvs.y * material.normal_tiling.y)).rgb - 1.0f) * splatmap_color.g;

		normal_texel.xy *= material.normal_strength;
		final_normal += normal_texel;
	}

	if(material.hasBlueChannelNormal == 1)
	{
		vec3 normal_texel = (2.0f * texture(material.blue_channel_normal, vec2(f_uvs.x * material.normal_tiling.x, f_uvs.y * material.normal_tiling.y)).rgb - 1.0f) * splatmap_color.b;

		normal_texel.xy *= material.normal_strength;
		final_normal += normal_texel;
	}

	normal = f_tbn * normalize(final_normal);

	vec3 lightDir = normalize(light.direction);
    float diff = max(0.0, dot(normal, lightDir)) * light.intensity;

	vec3 diffuse = vec3(0.0f);
	vec3 specular = vec3(0.0f);
	vec3 ambient = light.ambient * material.ambientColor;

	vec3 red_channel_diffuse = vec3(0.0f);
	vec3 red_channel_specular = vec3(0.0f);

	vec3 green_channel_diffuse = vec3(0.0f);
	vec3 green_channel_specular = vec3(0.0f);

	vec3 blue_channel_diffuse = vec3(0.0f);
	vec3 blue_channel_specular = vec3(0.0f);


	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0f), material.shininess) * material.shininess_strength;
	vec4 texel = vec4(0.0f);
	vec4 red_channel_diff = vec4(0.0f);
	vec4 green_channel_diff = vec4(0.0f);
	vec4 blue_channel_diff = vec4(0.0f);

	if(material.hasDiffuse == 1)
	{
		float base_ratio = 1.0f - (splatmap_color.r + splatmap_color.g + splatmap_color.b);
		texel = texture(material.diffuseMap, vec2(f_uvs.x * material.diffuse_tiling.x, f_uvs.y * material.diffuse_tiling.y)) * base_ratio;

//		if(texel.a <= 0.0f)
//			discard;

		diffuse = light.diffuse * material.diffuseColor * diff * vec3(texel);
		ambient = light.ambient + material.ambientColor * vec3(texel);
	}
	else {
		ambient = light.ambient + material.ambientColor;
		diffuse = light.diffuse * diff * material.diffuseColor;
	}

	if(material.hasSpecular == 1)
		specular = light.specular * material.specularColor * spec * vec3(texture(material.specularMap, vec2(f_uvs.x * material.specular_tiling.x, f_uvs.y * material.specular_tiling.y)));
	else
		specular = light.specular * spec * material.specularColor;

	if(material.hasRedChannelSpecular == 1)
	{
		vec4 red_channel_spec = texture(material.red_channel_specular, vec2(f_uvs.x * material.specular_tiling.x, f_uvs.y * material.specular_tiling.y)) * splatmap_color.r;
		red_channel_specular = light.specular * material.specularColor * spec * vec3(red_channel_spec);
	}

	if(material.hasGreenChannelSpecular == 1)
	{
		vec4 green_channel_spec = texture(material.green_channel_specular, vec2(f_uvs.x * material.specular_tiling.x, f_uvs.y * material.specular_tiling.y)) * splatmap_color.g;
		green_channel_specular = light.specular * material.specularColor * spec * vec3(green_channel_spec);
	}

	if(material.hasBlueChannelSpecular == 1)
	{
		vec4 blue_channel_spec = texture(material.blue_channel_specular, vec2(f_uvs.x * material.specular_tiling.x, f_uvs.y * material.specular_tiling.y)) * splatmap_color.b;
		blue_channel_specular = light.specular * material.specularColor * spec * vec3(blue_channel_spec);
	}

	if(material.hasRedChannelDiffuse == 1)
	{
		red_channel_diff = texture(material.red_channel_diffuse, vec2(f_uvs.x * material.diffuse_tiling.x, f_uvs.y * material.diffuse_tiling.y)) * splatmap_color.r;
		red_channel_diffuse = light.diffuse * material.diffuseColor * diff * vec3(blendTextures(texel, 1.0f, red_channel_diff, 1.0f));
	}
	
	if(material.hasGreenChannelDiffuse == 1)
	{
		green_channel_diff = texture(material.green_channel_diffuse, vec2(f_uvs.x * material.diffuse_tiling.x, f_uvs.y * material.diffuse_tiling.y)) * splatmap_color.g;
		green_channel_diffuse = light.diffuse * material.diffuseColor * diff * vec3(blendTextures(red_channel_diff, 1.0f, green_channel_diff, 1.0f));
	}
	
	if(material.hasBlueChannelDiffuse == 1)
	{
		blue_channel_diff = texture(material.blue_channel_diffuse, vec2(f_uvs.x * material.diffuse_tiling.x, f_uvs.y * material.diffuse_tiling.y)) * splatmap_color.b;
		blue_channel_diffuse = light.diffuse * material.diffuseColor * diff * vec3(blendTextures(green_channel_diff, 1.0f, blue_channel_diff, 1.0f));
	}

	float shadow = CalcShadows(f_posLightSpace, material, f_normal, f_uvs, f_tbn, f_position, lightPos);

	lighting = ambient + (0.8f - shadow) * (
		diffuse +
		
		red_channel_diffuse +
		green_channel_diffuse +
		blue_channel_diffuse +
		
		red_channel_specular +
		green_channel_specular +
		blue_channel_specular +

		specular);

    return lighting;
}

vec3 CalcPointLight(Point light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
	vec4 splatmap_color = vec4(0.0f);
	vec3 red_channel_diffuse = vec3(0.0f);
	vec3 red_channel_specular = vec3(0.0f);
	vec3 green_channel_diffuse = vec3(0.0f);
	vec3 green_channel_specular = vec3(0.0f);
	vec3 blue_channel_diffuse = vec3(0.0f);
	vec3 blue_channel_specular = vec3(0.0f);

	if(material.hasSplatmap == 1)
		splatmap_color = texture(material.splatmap, f_uvs);

    vec3 final_normal = vec3(0.0f);
	vec3 base_normal = vec3(0.0f);
	vec3 red_channel_normal = vec3(0.0f);
	vec3 green_channel_normal = vec3(0.0f);
	vec3 blue_channel_normal = vec3(0.0f);

	if(material.hasNormal == 1)
	{
		vec3 normal_texel = 2.0f * texture(material.normalMap, vec2(f_uvs.x * material.normal_tiling.x, f_uvs.y * material.normal_tiling.y)).rgb - 1.0f;

		normal_texel.xy *= material.normal_strength;
		final_normal += normal_texel;
	}

	if(material.hasRedChannelNormal == 1)
	{
		vec3 normal_texel = (2.0f * texture(material.red_channel_normal, vec2(f_uvs.x * material.normal_tiling.x, f_uvs.y * material.normal_tiling.y)).rgb - 1.0f) * splatmap_color.r;

		normal_texel.xy *= material.normal_strength;
		final_normal += normal_texel;
	}
	
	if(material.hasGreenChannelNormal == 1)
	{
		vec3 normal_texel = (2.0f * texture(material.green_channel_normal, vec2(f_uvs.x * material.normal_tiling.x, f_uvs.y * material.normal_tiling.y)).rgb - 1.0f) * splatmap_color.g;

		normal_texel.xy *= material.normal_strength;
		final_normal += normal_texel;
	}

	if(material.hasBlueChannelNormal == 1)
	{
		vec3 normal_texel = (2.0f * texture(material.blue_channel_normal, vec2(f_uvs.x * material.normal_tiling.x, f_uvs.y * material.normal_tiling.y)).rgb - 1.0f) * splatmap_color.b;

		normal_texel.xy *= material.normal_strength;
		final_normal += normal_texel;
	}

	normal = f_tbn * normalize(final_normal);

    vec3 lightDir = normalize(light.position - fragPos);
    float diff = max(dot(normal, lightDir), 0.0f) * light.intensity;

    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0f), material.shininess);

    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    

	vec3 diffuse = vec3(0.0f);
	vec3 specular = vec3(0.0f);

    vec3 ambient = light.ambient * material.ambientColor * vec3(texture(material.diffuseMap, vec2(f_uvs.x * material.diffuse_tiling.x, f_uvs.y * material.diffuse_tiling.y)));
    
	if(material.hasDiffuse == 1) {
		float base_ratio = 1.0f - (splatmap_color.r + splatmap_color.g + splatmap_color.b);
		diffuse = light.diffuse * material.diffuseColor * diff * vec3(texture(material.diffuseMap, vec2(f_uvs.x * material.diffuse_tiling.x, f_uvs.y * material.diffuse_tiling.y))) * base_ratio;
	} else
		diffuse = light.diffuse * diff * material.diffuseColor;

	if(material.hasSpecular == 1)
		specular = light.specular * material.specularColor * spec * vec3(texture(material.specularMap, vec2(f_uvs.x * material.specular_tiling.x, f_uvs.y * material.specular_tiling.y)));
	else
		specular = light.specular * spec * material.specularColor;

	if(material.hasRedChannelSpecular == 1)
	{
		vec4 red_channel_spec = texture(material.red_channel_specular, vec2(f_uvs.x * material.specular_tiling.x, f_uvs.y * material.specular_tiling.y)) * splatmap_color.r;
		red_channel_specular = light.specular * material.specularColor * spec * vec3(red_channel_spec);
	}

	if(material.hasGreenChannelSpecular == 1)
	{
		vec4 green_channel_spec = texture(material.green_channel_specular, vec2(f_uvs.x * material.specular_tiling.x, f_uvs.y * material.specular_tiling.y)) * splatmap_color.g;
		green_channel_specular = light.specular * material.specularColor * spec * vec3(green_channel_spec);
	}

	if(material.hasBlueChannelSpecular == 1)
	{
		vec4 blue_channel_spec = texture(material.blue_channel_specular, vec2(f_uvs.x * material.specular_tiling.x, f_uvs.y * material.specular_tiling.y)) * splatmap_color.b;
		blue_channel_specular = light.specular * material.specularColor * spec * vec3(blue_channel_spec);
	}

	if(material.hasRedChannelDiffuse == 1)
	{
		vec4 red_channel_diff = texture(material.red_channel_diffuse, vec2(f_uvs.x * material.diffuse_tiling.x, f_uvs.y * material.diffuse_tiling.y)) * splatmap_color.r;
		red_channel_diffuse = light.diffuse * material.diffuseColor * diff * vec3(red_channel_diff);
	}
	
	if(material.hasGreenChannelDiffuse == 1)
	{
		vec4 green_channel_diff = texture(material.green_channel_diffuse, vec2(f_uvs.x * material.diffuse_tiling.x, f_uvs.y * material.diffuse_tiling.y)) * splatmap_color.g;
		green_channel_diffuse = light.diffuse * material.diffuseColor * diff * vec3(green_channel_diff);
	}
	
	if(material.hasBlueChannelDiffuse == 1)
	{
		vec4 blue_channel_diff = texture(material.blue_channel_diffuse, vec2(f_uvs.x * material.diffuse_tiling.x, f_uvs.y * material.diffuse_tiling.y)) * splatmap_color.b;
		blue_channel_diffuse = light.diffuse * material.diffuseColor * diff * vec3(blue_channel_diff);
	}

    ambient  *= attenuation;
    diffuse  *= attenuation;
    specular *= attenuation;

    return (
		ambient +

		blue_channel_diffuse +
		green_channel_diffuse +
		red_channel_diffuse + 

		diffuse + 

		blue_channel_specular +
		green_channel_specular +
		red_channel_specular + 

		specular);
}

vec3 CalcSpotLight(Spot light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
	vec4 splatmap_color = vec4(0.0f);
	vec3 red_channel_diffuse = vec3(0.0f);
	vec3 red_channel_specular = vec3(0.0f);
	vec3 green_channel_diffuse = vec3(0.0f);
	vec3 green_channel_specular = vec3(0.0f);
	vec3 blue_channel_diffuse = vec3(0.0f);
	vec3 blue_channel_specular = vec3(0.0f);

	if(material.hasSplatmap == 1)
		splatmap_color = texture(material.splatmap, f_uvs);

	if(material.hasNormal == 1)
	{
		vec3 normal_texel = 2.0f * texture(material.normalMap,  vec2(f_uvs.x * material.normal_tiling.x, f_uvs.y * material.normal_tiling.y)).rgb - 1.0f;
		normal_texel.xy *= material.normal_strength;
		normal_texel = normalize(normal_texel);
		normal = normalize(f_tbn * normal_texel);
	}		

	vec3 final_normal = vec3(0.0f);
	vec3 base_normal = vec3(0.0f);
	vec3 red_channel_normal = vec3(0.0f);
	vec3 green_channel_normal = vec3(0.0f);
	vec3 blue_channel_normal = vec3(0.0f);

	if(material.hasNormal == 1)
	{
		vec3 normal_texel = 2.0f * texture(material.normalMap, vec2(f_uvs.x * material.normal_tiling.x, f_uvs.y * material.normal_tiling.y)).rgb - 1.0f;

		normal_texel.xy *= material.normal_strength;
		final_normal += normal_texel;
	}

	if(material.hasRedChannelNormal == 1)
	{
		vec3 normal_texel = (2.0f * texture(material.red_channel_normal, vec2(f_uvs.x * material.normal_tiling.x, f_uvs.y * material.normal_tiling.y)).rgb - 1.0f) * splatmap_color.r;

		normal_texel.xy *= material.normal_strength;
		final_normal += normal_texel;
	}
	
	if(material.hasGreenChannelNormal == 1)
	{
		vec3 normal_texel = (2.0f * texture(material.green_channel_normal, vec2(f_uvs.x * material.normal_tiling.x, f_uvs.y * material.normal_tiling.y)).rgb - 1.0f) * splatmap_color.g;

		normal_texel.xy *= material.normal_strength;
		final_normal += normal_texel;
	}

	if(material.hasBlueChannelNormal == 1)
	{
		vec3 normal_texel = (2.0f * texture(material.blue_channel_normal, vec2(f_uvs.x * material.normal_tiling.x, f_uvs.y * material.normal_tiling.y)).rgb - 1.0f) * splatmap_color.b;

		normal_texel.xy *= material.normal_strength;
		final_normal += normal_texel;
	}

	normal = f_tbn * normalize(final_normal);

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
	{
		float base_ratio = 1.0f - (splatmap_color.r + splatmap_color.g + splatmap_color.b);
		diffuse = light.diffuse * material.diffuseColor * diff * vec3(texture(material.diffuseMap, vec2(f_uvs.x * material.diffuse_tiling.x, f_uvs.y * material.diffuse_tiling.y))) * base_ratio;
	} else
		diffuse = light.diffuse * diff * material.diffuseColor;

	if(material.hasSpecular == 1)
		specular = light.specular * material.specularColor * spec * vec3(texture(material.specularMap, vec2(f_uvs.x * material.specular_tiling.x, f_uvs.y * material.specular_tiling.y)));
	else
		specular = light.specular * spec * material.specularColor;

	if(material.hasRedChannelSpecular == 1)
	{
		vec4 red_channel_spec = texture(material.red_channel_specular, vec2(f_uvs.x * material.specular_tiling.x, f_uvs.y * material.specular_tiling.y)) * splatmap_color.r;
		red_channel_specular = light.specular * material.specularColor * spec * vec3(red_channel_spec);
	}

	if(material.hasGreenChannelSpecular == 1)
	{
		vec4 green_channel_spec = texture(material.green_channel_specular, vec2(f_uvs.x * material.specular_tiling.x, f_uvs.y * material.specular_tiling.y)) * splatmap_color.g;
		green_channel_specular = light.specular * material.specularColor * spec * vec3(green_channel_spec);
	}

	if(material.hasBlueChannelSpecular == 1)
	{
		vec4 blue_channel_spec = texture(material.blue_channel_specular, vec2(f_uvs.x * material.specular_tiling.x, f_uvs.y * material.specular_tiling.y)) * splatmap_color.b;
		blue_channel_specular = light.specular * material.specularColor * spec * vec3(blue_channel_spec);
	}

	if(material.hasRedChannelDiffuse == 1)
	{
		vec4 red_channel_diff = texture(material.red_channel_diffuse, vec2(f_uvs.x * material.diffuse_tiling.x, f_uvs.y * material.diffuse_tiling.y)) * splatmap_color.r;
		red_channel_diffuse = light.diffuse * diff * vec3(red_channel_diff);
	}
	
	if(material.hasGreenChannelDiffuse == 1)
	{
		vec4 green_channel_diff = texture(material.green_channel_diffuse, vec2(f_uvs.x * material.diffuse_tiling.x, f_uvs.y * material.diffuse_tiling.y)) * splatmap_color.g;
		green_channel_diffuse = light.diffuse * diff * vec3(green_channel_diff);
	}
	
	if(material.hasBlueChannelDiffuse == 1)
	{
		vec4 blue_channel_diff = texture(material.blue_channel_diffuse, vec2(f_uvs.x * material.diffuse_tiling.x, f_uvs.y * material.diffuse_tiling.y)) * splatmap_color.b;
		blue_channel_diffuse = light.diffuse * diff * vec3(blue_channel_diff);
	}

    ambient  *= attenuation * intensity;
    diffuse  *= attenuation * intensity;
    specular *= attenuation * intensity;

    return (
		ambient + diffuse +

		red_channel_diffuse + 
		green_channel_diffuse +
		blue_channel_diffuse +
		
		red_channel_specular +
		green_channel_specular + 
		blue_channel_specular +
		
		specular);
}
