#version 330

in vec3 position;
out vec3 fUvs;

uniform mat4 proj;
uniform mat4 view;
uniform mat4 model;

void main()
{
    fUvs = normalize(position);
    gl_Position = proj * view * model * vec4(position, 1.0);
}