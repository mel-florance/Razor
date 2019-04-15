#version 330 core

out vec4 FragColor;

#define MAX_LIGHTS 1

layout (std140) uniform shader_data
{
	vec3 cameras_positions;
	vec3 lights_positions;
};

struct Material {
	sampler2D diffuse;
	sampler2D specular;
	float shininess;
};

struct Directional {
	vec3 direction;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

struct Point {
	vec3 position;
	float constant;
	float linear;
	float quadratic;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

struct Spot {
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

uniform vec3 viewPosition; 

uniform Directional directionalLight;
uniform Point pointLights[MAX_LIGHTS];
uniform Spot spotLight;
uniform Material material;

vec3 CalcDirectionalLight(Directional light, vec3 normal, vec3 viewDir);
vec3 CalcPointLight(Point light, vec3 normal, vec3 fragPos, vec3 viewDir);
vec3 CalcSpotLight(Spot light, vec3 normal, vec3 fragPos, vec3 viewDir);

void main() 
{
	vec3 norm = normalize(f_normal);
    vec3 viewDir = normalize(viewPosition - f_position);

	vec3 result = CalcDirectionalLight(directionalLight, norm, viewDir);

	for(int i = 0; i < MAX_LIGHTS; i++)
        result += CalcPointLight(pointLights[i], norm, f_position, viewDir);    

    result += CalcSpotLight(spotLight, norm, f_position, viewDir);    
    
    FragColor = vec4(result, 1.0);
	float gamma = 2.2;
    FragColor.rgb = pow(FragColor.rgb, vec3(1.0/ gamma));
}

vec3 CalcDirectionalLight(Directional light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-light.direction);

    float diff = max(dot(normal, lightDir), 0.0);

    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

    vec3 ambient = light.ambient * vec3(texture(material.diffuse, f_uvs));
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, f_uvs));
    vec3 specular = light.specular * spec * vec3(texture(material.specular, f_uvs));

    return (ambient + diffuse + specular);
}

vec3 CalcPointLight(Point light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);

    float diff = max(dot(normal, lightDir), 0.0);

    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    

    vec3 ambient = light.ambient * vec3(texture(material.diffuse, f_uvs));
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, f_uvs));
    vec3 specular = light.specular * spec * vec3(texture(material.specular, f_uvs));

    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;

    return (ambient + diffuse + specular);
}

vec3 CalcSpotLight(Spot light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);

    float diff = max(dot(normal, lightDir), 0.0);

    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    

    float theta = dot(lightDir, normalize(-light.direction)); 
    float epsilon = light.inner_cutoff - light.outer_cutoff;
    float intensity = clamp((theta - light.outer_cutoff) / epsilon, 0.0, 1.0);

    vec3 ambient = light.ambient * vec3(texture(material.diffuse, f_uvs));
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, f_uvs));
    vec3 specular = light.specular * spec * vec3(texture(material.specular, f_uvs));

    ambient *= attenuation * intensity;
    diffuse *= attenuation * intensity;
    specular *= attenuation * intensity;

    return (ambient + diffuse + specular);
}