#version 330 core

const float PI = 3.14159265;

in vec2 f_uvs;
in vec4 f_position;
out vec4 FragColor;

uniform sampler2D target;
uniform float resolution;
uniform float radius;
uniform vec2 dir;

void main()
{
	vec4 sum = vec4(0.0);
	float blur = radius / resolution; 
    
	sum += texture(target, vec2(f_uvs.x - 4.0 * blur * dir.x, f_uvs.y - 4.0 * blur * dir.y)) * 0.0162162162;
	sum += texture(target, vec2(f_uvs.x - 3.0 * blur * dir.x, f_uvs.y - 3.0 * blur * dir.y)) * 0.0540540541;
	sum += texture(target, vec2(f_uvs.x - 2.0 * blur * dir.x, f_uvs.y - 2.0 * blur * dir.y)) * 0.1216216216;
	sum += texture(target, vec2(f_uvs.x - 1.0 * blur * dir.x, f_uvs.y - 1.0 * blur * dir.y)) * 0.1945945946;
	
	sum += texture2D(target, vec2(f_uvs.x, f_uvs.y)) * 0.2270270270;
	
	sum += texture2D(target, vec2(f_uvs.x + 1.0 * blur * dir.x, f_uvs.y + 1.0 * blur * dir.y)) * 0.1945945946;
	sum += texture2D(target, vec2(f_uvs.x + 2.0 * blur * dir.x, f_uvs.y + 2.0 * blur * dir.y)) * 0.1216216216;
	sum += texture2D(target, vec2(f_uvs.x + 3.0 * blur * dir.x, f_uvs.y + 3.0 * blur * dir.y)) * 0.0540540541;
	sum += texture2D(target, vec2(f_uvs.x + 4.0 * blur * dir.x, f_uvs.y + 4.0 * blur * dir.y)) * 0.0162162162;

	FragColor = vec4(sum.rgb, 1.0);
}