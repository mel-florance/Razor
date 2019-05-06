#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 uvs;
layout (location = 3) in vec3 tangent;

out vec2 f_uvs;
out vec3 f_normal;
out vec3 f_position;
out vec3 f_tangent;
out vec4 f_posLightSpace;
out mat3 f_tbn;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 lightSpaceMatrix;

void main() 
{
	f_uvs = uvs;    
	f_position = vec3(model * vec4(position, 1.0));
	f_normal = mat3(transpose(inverse(model))) * normal;
	f_tangent = tangent;
	f_posLightSpace = lightSpaceMatrix * vec4(f_position, 1.0);

	vec3 T = normalize(vec3(model * vec4(tangent,   0.0)));
	vec3 N = normalize(vec3(model * vec4(normal,    0.0)));
	T = normalize(T - dot(T, N) * N);
	vec3 B = normalize(cross(T, N));

	f_tbn = mat3(T, B, N);

	gl_Position = projection * view * model * vec4(position, 1.0);
}