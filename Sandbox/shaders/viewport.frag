#version 330 core

in vec2 f_uvs;
out vec4 FragColor;

uniform sampler2D target;

void main()
{
    FragColor = vec4(texture(target, f_uvs).rgb, 1.0);
} 