#version 430
in vec3 world_position;
in vec3 world_normal;
in vec3 tex_coords;

uniform vec3 light_pos;
uniform float far;

layout(binding = 2) uniform samplerCube shadow_map;

layout(location = 0) out vec4 out_color;

void main()
{
    out_color = vec4(texture(shadow_map, vec3(tex_coords.x, tex_coords.y, tex_coords.z)).xyz, 1.0);
}  