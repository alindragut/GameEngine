#version 430

layout(location = 1) in vec3 vn;
layout(location = 2) in vec3 wp;
layout(location = 3) in vec3 wn;

uniform int toggleNormals;
uniform samplerCube texture_cubemap;
uniform vec3 camera_position;
uniform int wireframe;
uniform int ref;

layout(location = 0) out vec4 out_color;

vec3 myReflect()
{
	return vec3(texture(texture_cubemap, reflect
		(vec3(normalize(wp - camera_position)),
		wn)));
}

vec3 myRefract(float refractive_index)
{
	return vec3(texture(texture_cubemap, refract
		(vec3(normalize(wp - camera_position)),
		wn, 1.0 / refractive_index)));;
}

void main()
{
	if (toggleNormals == 1) {
		out_color = vec4(vn, 1);
	} else {
		if (wireframe == 1) {
			out_color = vec4(1);
		} else {
			if (ref == 1) {
				out_color =  vec4(myReflect(), 1);
			} else {
				out_color =  vec4(myRefract(1), 1);
			}
		}
	}
}