#version 330 core

#define MAX_DIRECTIONAL_LIGHTS 1
#define MAX_POINT_LIGHTS 1
#define MAX_SPOT_LIGHTS 1

out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gAlbedoSpec;

struct PointLight 
{
    vec3 position;
    float linear;
    float quadratic;
    float intensity;
    vec3 diffuse;
//    vec3 ambient;
    vec3 specular;
};

uniform PointLight point_lights[32];
uniform vec3 viewPos;

void main()
{             
	vec2 uvs = vec2(TexCoords.x, TexCoords.y * -1.0);

    // Retrieve data from gbuffer
    vec3 FragPos = texture(gPosition, uvs).rgb;
    vec3 Normal = texture(gNormal, uvs).rgb;
    vec3 Diffuse = texture(gAlbedoSpec, uvs).rgb;
    float Specular = texture(gAlbedoSpec, uvs).a;

    vec3 lighting = vec3(0.0);
    vec3 viewDir  = normalize(viewPos - FragPos);

    for(int i = 0; i < 32; ++i)
    {
		if (point_lights[i].intensity > 0)
		{
			// Diffuse
			vec3 lightDir = normalize(point_lights[i].position - FragPos) * point_lights[i].intensity;
			vec3 diffuse = max(dot(Normal, lightDir), 0.0) * Diffuse * point_lights[i].diffuse;

			// Specular
			vec3 halfwayDir = normalize(lightDir + viewDir);  
			float spec = pow(max(dot(Normal, halfwayDir), 0.0), 16.0);
			vec3 specular = point_lights[i].diffuse * spec * Specular * point_lights[i].specular;

			// Attenuation
			float dist = length(point_lights[i].position - FragPos);
			float attenuation = 1.0 / (1.0 + point_lights[i].linear * dist + point_lights[i].quadratic * dist * dist);

			diffuse *= attenuation;
			specular *= attenuation;

			lighting += diffuse + specular;        
		}
    }

	FragColor = vec4(lighting, 1.0);
}
