#version 430

layout(location = 0) in vec3 world_position;
layout(location = 1) in vec3 world_normal;

uniform sampler2D texture_1;
uniform samplerCube texture_cubemap;

uniform vec3 camera_position;

layout(location = 0) out vec4 out_color;

vec4 myReflect()
{
	vec4 culoarefragment = texture(texture_cubemap, reflect(normalize(world_position - camera_position), world_normal));

	return culoarefragment;
}

vec4 myRefract(float refractive_index)
{
	vec4 culoareFragment = texture(texture_cubemap, refract(normalize(world_position - camera_position), world_normal, 1.0/refractive_index));

	return culoareFragment;
}

void main()
{
    out_color = vec4(myReflect()+myRefract(1.33));

}
