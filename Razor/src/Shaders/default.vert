#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 uvs;

out vec2 f_uvs;
out vec3 f_normal;
out vec3 f_position;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() 
{
	f_uvs = uvs;    
	f_position = vec3(model * vec4(position, 1.0));
	f_normal = mat3(transpose(inverse(model))) * normal;

	gl_Position = projection * view * model * vec4(position, 1.0);
}