#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 uvs;
layout (location = 2) in vec3 normal;
layout (location = 3) in vec3 tangent;

out vec2 TexCoords;
out vec3 WorldPos;
out vec3 Normal;
out vec3 Tangent;
out mat4 Model;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

void main()
{
    TexCoords = uvs;
    WorldPos = vec3(model * vec4(position, 1.0));
    Normal = mat3(model) * normal;   
    Tangent = tangent;   
    Model = model;   

    gl_Position =  projection * view * vec4(WorldPos, 1.0);
}