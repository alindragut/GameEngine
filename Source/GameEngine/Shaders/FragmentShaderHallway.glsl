#version 430
layout(location = 0) in vec2 texture_coord;
layout(location = 1) in vec3 v_normal_out;

uniform sampler2D texture_1;

layout(location = 0) out vec4 out_color;

void main()
{
	vec4 color = vec4(0, 0, 1, 1);

	out_color = color;
}