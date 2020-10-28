#version 330 core

layout (location = 0) in vec4 position;
layout (location = 1) in vec2 uvs;

uniform vec2 pixelSize;

out vec2 f_uvs;

void main()
{
	f_uvs = uvs;
	gl_Position = position;
}