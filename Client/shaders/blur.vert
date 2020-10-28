#version 330 core

layout (location = 0) in vec4 position;
layout (location = 1) in vec2 uvs;

out vec2 f_uvs;
out vec4 f_position;

void main()
{
	f_uvs = uvs;
	f_position = position;
    gl_Position = position;
}