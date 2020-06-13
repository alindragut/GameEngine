#version 430

layout(location = 0) in vec3 v_position;
layout(location = 1) in vec3 v_normal;
layout(location = 2) in vec2 v_texture_coord;

// Uniform properties
uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;
uniform mat4 Light_Projection;
uniform mat4 Light_View;

layout(location = 0) out vec2 texture_coord;
layout(location = 1) out vec3 v_normal_out;
layout(location = 2) out vec4 light_space_position;
layout(location = 3) out vec3 v_pos;

void main()
{
	texture_coord = v_texture_coord;
	v_pos = v_position;

	gl_Position = Projection * View * Model * vec4(v_position, 1);

	v_normal_out = v_normal;

	light_space_position = Light_Projection * Light_View * Model * vec4(v_position, 1);
}
