#version 140

in vec2 position;

uniform mat4 projection;
uniform mat4 model_view;

void main(void)
{
	gl_Position = projection * model_view * vec4(position, 0.0, 1.0);
}
