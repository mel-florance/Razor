#version 330 core

layout (location = 0) in vec2 pos;
layout (location = 1) in vec2 uvs;

out vec2 f_uvs;

void main()
{
    f_uvs = uvs;
    gl_Position = vec4(pos.x, pos.y, 0.0, 1.0); 
}  