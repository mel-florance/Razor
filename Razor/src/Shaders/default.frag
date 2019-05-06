#version 330 core

out vec4 FragColor;

#define MAX_DIRECTIONAL_LIGHTS 1
#define MAX_POINT_LIGHTS 1
#define MAX_SPOT_LIGHTS 1

struct Material
{
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
};

struct Directional
{
	vec3 direction;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
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
};

struct Spot 
{
	vec3 position;
    vec3 direction;
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


float CalcShadows(vec4 fragPosLightSpace)
{
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;

    projCoords = projCoords * 0.5 + 0.5;

    float closestDepth = texture(material.shadowMap, projCoords.xy).r; 

    float currentDepth = projCoords.z;

    vec3 normal = normalize(f_normal);

	if(material.hasNormal == 1)
	{
		vec3 normal_texel = 2.0f * texture(material.normalMap, f_uvs).rgb - 1.0f;
		normal_texel.xy *= material.normal_strength;
		normal_texel = normalize(normal_texel);
		normal = normalize(f_tbn * normal_texel);
	}

    vec3 lightDir = normalize(lightPos - f_position);
    float bias = max(0.02 * (1.0 - dot(normal, lightDir)), 0.005);

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
//    FragColor.rgb = pow(FragColor.rgb, vec3(1.0f / 2.2f));
}

vec3 CalcDirectionalLight(Directional light, vec3 normal, vec3 viewDir)
{
	vec3 lighting = vec3(0.0); 
    
	vec3 lightDir = normalize(light.direction);
    float diff = max(0.0, dot(normal, lightDir));

	vec3 diffuse = vec3(0.0f);
	vec3 specular = vec3(0.0f);
	vec3 ambient = light.ambient * material.ambientColor;

	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0f), material.shininess) * material.shininess_strength;

	if(material.hasNormal == 1)
	{
		vec3 normal_texel = 2.0f * texture(material.normalMap, f_uvs).rgb - 1.0f;
		normal_texel.xy *= material.normal_strength;
		normal_texel = normalize(normal_texel);
		normal = normalize(f_tbn * normal_texel);
	}

	if(material.hasDiffuse == 1)
	{
		vec4 texel = texture(material.diffuseMap, f_uvs);

		if(texel.a <= 0.0f)
			discard;

		diffuse = light.diffuse * diff * vec3(texel);
		ambient = light.ambient * vec3(texture(material.diffuseMap, f_uvs));
	}
	else {
		ambient = light.ambient * material.ambientColor;
		diffuse = light.diffuse * diff * material.diffuseColor;
	}
	
	if(material.hasSpecular == 1)
		specular = light.specular * spec * vec3(texture(material.specularMap, f_uvs));
	else
		specular = light.specular * spec * material.specularColor;

	float shadow = CalcShadows(f_posLightSpace);
	lighting = ambient + (1.0 - shadow) * (diffuse + specular);

    return lighting;
}

vec3 CalcPointLight(Point light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);
    float diff = max(dot(normal, lightDir), 0.0f);

	if(material.hasNormal == 1)
	{
		vec3 normal_texel = 2.0f * texture(material.normalMap, f_uvs).rgb - 1.0f;
		normal_texel.xy *= material.normal_strength;
		normal_texel = normalize(normal_texel);
		normal = normalize(f_tbn * normal_texel);
	}

    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0f), material.shininess);

    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    

	vec3 diffuse = vec3(0.0f);
	vec3 specular = vec3(0.0f);

    vec3 ambient = light.ambient  * vec3(texture(material.diffuseMap, f_uvs));
    
	if(material.hasDiffuse == 1)
		diffuse = light.diffuse  * diff * vec3(texture(material.diffuseMap, f_uvs));
	else
		diffuse = light.diffuse * diff * material.diffuseColor;

	if(material.hasSpecular == 1)
		specular = light.specular * spec * vec3(texture(material.specularMap, f_uvs));
	else
		specular = light.specular * spec * material.specularColor;

    ambient  *= attenuation;
    diffuse  *= attenuation;
    specular *= attenuation;

    return (ambient + diffuse + specular);
}

vec3 CalcSpotLight(Spot light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);
    float diff = max(dot(normal, lightDir), 0.0f);

	if(material.hasNormal == 1)
	{
		vec3 normal_texel = 2.0f * texture(material.normalMap, f_uvs).rgb - 1.0f;
		normal_texel.xy *= material.normal_strength;
		normal_texel = normalize(normal_texel);
		normal = normalize(f_tbn * normal_texel);
	}		

    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0f), material.shininess);

    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    

    float theta     = dot(lightDir, normalize(-light.direction)); 
    float epsilon   = light.inner_cutoff - light.outer_cutoff;
    float intensity = clamp((theta - light.outer_cutoff) / epsilon, 0.0f, 1.0f);

    vec3 ambient  = light.ambient  * vec3(texture(material.diffuseMap, f_uvs));
	
	vec3 diffuse = vec3(0.0f);
	vec3 specular = vec3(0.0f);

	if(material.hasDiffuse == 1)
		diffuse = light.diffuse * diff * vec3(texture(material.diffuseMap, f_uvs));
	else
		diffuse = light.diffuse * diff * material.diffuseColor;

	if(material.hasSpecular == 1)
		specular = light.specular * spec * vec3(texture(material.specularMap, f_uvs));
	else
		specular = light.specular * spec * material.specularColor;

    ambient  *= attenuation * intensity;
    diffuse  *= attenuation * intensity;
    specular *= attenuation * intensity;

    return (ambient + diffuse + specular);
}
