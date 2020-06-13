#version 430

layout (location = 0) in vec3 v_position;

uniform mat4 Model;

void main()
{
    gl_Position = Model * vec4(v_position, 1.0);
}  