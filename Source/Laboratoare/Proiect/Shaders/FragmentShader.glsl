#version 430

layout(location = 0) in vec2 texture_coord;
layout(location = 0) out vec4 out_color;

uniform sampler2D txt;

void main()
{
	out_color = texture(txt, texture_coord);
}