#version 430
layout(location = 0) in vec2 texture_coord;
layout(location = 1) in vec3 v_normal_out;
layout(location = 2) in vec4 light_space_position;
layout(location = 3) in vec3 v_pos;

layout(binding = 2) uniform sampler2DShadow shadow_map;

layout(location = 0) out vec4 out_color;

void main()
{	
	vec3 pos = (light_space_position.xyz / light_space_position.w) * 0.5 + 0.5;
	pos.z = clamp(pos.z, 0, 1);
	float bias = 0.002 * tan(acos(dot(normalize(v_normal_out), normalize(vec3(15, 21, 15)))));
	bias = clamp(bias, 0, 0.01);
	pos.z -= bias;
	float shadow = 0.0;
	vec2 texelSize = 1.0 / textureSize(shadow_map, 0);
	for (int i = -1; i <= 1; i++) {
		for (int j = -1; j <= 1; j++) {
			vec3 newPos = vec3(pos.xy + vec2(i, j) * texelSize, pos.z);
			shadow += texture(shadow_map, newPos, 0);
		}
	}

	shadow /= 9.0;

	

	vec3 color = shadow * vec3(0.3, 0.4, 0.5);

	out_color = vec4(color, 1.0);

}