#version 430

layout(location = 0) in vec3 v_position;

uniform mat4 View;
uniform mat4 Projection;

layout(location = 0) out vec3 v_pos;

void main()
{


	v_pos = v_position;
	gl_Position = (Projection * View * vec4(v_position, 1)).xyww;

}
