#version 330 core

layout (location = 0) in vec3 position;

uniform mat4 lightViewMatrix;
uniform mat4 orthoProjectionMatrix;
uniform mat4 model;

void main()
{
    gl_Position = orthoProjectionMatrix * lightViewMatrix * model * vec4(position, 1.0);
}