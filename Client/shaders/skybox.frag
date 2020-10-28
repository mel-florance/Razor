#version 330

in vec3 fUvs;
out vec4 out_Color;

uniform samplerCube cubemap;

uniform vec4 clearColor;
uniform vec2 limits;

void main()
{
    vec4 texture1 = texture(cubemap, fUvs);

	float factor = (fUvs.y - limits.x) / (limits.y - limits.x);
	factor = clamp(factor, 0.0, 1.0);

	out_Color = mix(clearColor, texture1, factor);
}