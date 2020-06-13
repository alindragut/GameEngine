#version 430
in vec4 frag_pos;

uniform vec3 light_pos;
uniform float far;

void main()
{
    gl_FragDepth = length(frag_pos.xyz - light_pos) / far;
}  