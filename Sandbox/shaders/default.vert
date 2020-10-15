#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 uvs;
layout (location = 3) in vec3 tangent;
layout (location = 4) in mat4 instanceMatrix;

out vec2 f_uvs;
out vec3 f_normal;
out vec3 f_position;
out vec3 f_tangent;
out vec4 f_posLightSpace;
out mat3 f_tbn;
out float f_distance;

uniform int instanced;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 lightSpaceMatrix;

mat3 calcTBNMatrix(mat4 model)
{
	vec3 T = normalize(vec3(model * vec4(tangent,   0.0)));
	vec3 N = normalize(vec3(model * vec4(normal,    0.0)));
	T = normalize(T - dot(T, N) * N);
	vec3 B = normalize(cross(T, N));

	return mat3(T, B, N);
}

void main() 
{
	mat4 modelMatrix = instanced == 1 ? instanceMatrix : model;

	f_uvs = uvs;    
	f_position = vec3(modelMatrix * vec4(position, 1.0));
	f_normal = mat3(transpose(inverse(modelMatrix))) * normal;
	f_tangent = tangent;
//	f_tangent = vec3(modelMatrix * vec4(tangent, 1.0));
	f_posLightSpace = lightSpaceMatrix * vec4(f_position, 1.0);
	f_distance = length(vec4(view * modelMatrix * vec4(position, 1.0)).xyz);
	f_tbn = calcTBNMatrix(modelMatrix);

	gl_Position = projection * view * modelMatrix * vec4(position, 1.0);
}