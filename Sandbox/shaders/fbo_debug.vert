#version 330 core

layout (location = 0) in vec4 position;
layout (location = 1) in vec2 uvs;

out vec2 TexCoords;

void main()
{
    gl_Position = position;
    TexCoords = uvs;
}
 