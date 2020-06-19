#version 430

layout (location = 0) in vec3 v_position;
layout(location = 3) in mat4 instanceMatrix;

void main()
{
    gl_Position = instanceMatrix * vec4(v_position, 1.0);
}  