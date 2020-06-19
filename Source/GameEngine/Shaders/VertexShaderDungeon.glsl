#version 430

layout(location = 0) in vec3 v_position;
layout(location = 1) in vec3 v_normal;
layout(location = 2) in vec2 v_texture_coord;
layout(location = 3) in mat4 instanceMatrix;

uniform mat4 View;
uniform mat4 Projection;
uniform mat4 Light_View;
uniform mat4 Light_Proj;
uniform float interior;

out vec3 world_position;
out vec3 world_normal;
out vec2 tex_coords;
out vec4 light_space_position;

void main()
{
	world_position = (instanceMatrix * vec4(v_position, 1.0)).xyz;
	world_normal = mat3(transpose(inverse(instanceMatrix))) * interior * v_normal;
	tex_coords = v_texture_coord;

   gl_Position = Projection * View * instanceMatrix * vec4(v_position, 1.0);
   light_space_position = Light_Proj * Light_View * instanceMatrix * vec4(v_position, 1);
}  