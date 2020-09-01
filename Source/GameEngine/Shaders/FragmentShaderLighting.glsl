#version 430
in vec3 world_position;
in vec3 world_normal;
in vec2 tex_coords;
in vec4 light_space_position;

uniform vec3 light_pos;
uniform vec3 view_pos;
uniform float far;

layout(binding = 0) uniform sampler2D txt;
layout(binding = 1) uniform samplerCube shadow_cube;
layout(binding = 2) uniform sampler2DShadow shadow_map;


layout(location = 0) out vec4 out_color;

float calculatePointShadow() {
	vec3 dir = world_position - light_pos;
	float bias = 0.05;
	return length(dir) - bias > texture(shadow_cube, dir).r * far ? 0.0 : 1.0;
}

float calculateDirShadow() {
	vec3 pos = (light_space_position.xyz / light_space_position.w) * 0.5 + 0.5;
	pos.z = clamp(pos.z, 0.0 , 1.0);
	float bias = 0.002;
	bias = clamp(bias, 0.0, 0.01);
	pos.z -= bias;
	float shadow = 0.0;
	vec2 texelSize = 1.0 / textureSize(shadow_map, 0);
	for (int i = -1; i <= 1; i++) {
		for (int j = -1; j <= 1; j++) {
			vec3 newPos = vec3(pos.xy + vec2(i, j) * texelSize, pos.z);
			shadow += texture(shadow_map, newPos, 0);
		}
	}

	return shadow / 9.0;
}

float calculateDirLight(vec3 normal, vec3 view_dir)
{
	vec3 light_dir = normalize(vec3(1));
	vec3 reflect_dir = reflect(-light_dir, normal);
	
	float diffuse = max(dot(normal, light_dir), 0.0);
	float specular = 1 * pow(max(dot(view_dir, reflect_dir), 0.0), 32);

	return specular + diffuse;
}

float calculatePointLight(vec3 normal, vec3 view_dir)
{
	vec3 light_dir = normalize(light_pos - world_position);
	vec3 reflect_dir = reflect(-light_dir, normal);

	float diffuse = max(dot(normal, light_dir), 0.0);
	float specular = 1 * pow(max(dot(view_dir, reflect_dir), 0.0), 32);
	float attenuation = 1 / (pow(distance(light_pos, world_position), 2) + 1);

	return (specular + diffuse) * attenuation;
}


void main()
{
	float ambient = 0.15f;

	vec3 normal = normalize(world_normal);
	vec3 view_dir = normalize(view_pos - world_position);

	vec3 diff = texture(txt, tex_coords).xyz;

	vec3 color1 = diff * calculatePointShadow() * calculatePointLight(normal, view_dir);
	vec3 color2 = diff * calculateDirShadow() * calculateDirLight(normal, view_dir);

	vec3 color = color1 + color2;

	out_color = vec4(color, 1.0);
}  