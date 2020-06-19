#version 430
layout(location = 0) in vec3 v_pos;



layout(binding = 3) uniform samplerCube txt;

layout(location = 0) out vec4 out_color;

void main()
{
	out_color = texture(txt, v_pos);
}