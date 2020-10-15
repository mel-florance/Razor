#version 140

out vec4 FragColor;

uniform vec3 color;

void main(void)
{
	FragColor = vec4(color, 1.0);
}
