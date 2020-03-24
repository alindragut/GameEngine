#version 410

layout(location = 0) in vec3 v_position;
layout(location = 1) in vec2 v_texture_coord;
layout(location = 2) in vec3 v_normals;
layout(location = 3) in ivec4 boneIDs;
layout(location = 4) in vec4  weights;

layout(location = 0) out vec2 texture_coord;

uniform mat4 WVP;
uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;
uniform mat4 Bones[206];

void main()
{
	
	mat4 BoneTransform = Bones[boneIDs[0]] * weights[0];
	BoneTransform += Bones[boneIDs[1]] * weights[1];
	BoneTransform += Bones[boneIDs[2]] * weights[2];
	BoneTransform += Bones[boneIDs[3]] * weights[3];

	vec4 PosL = BoneTransform * vec4(v_position, 1.0);
	gl_Position = Projection * View * Model * PosL;
	texture_coord = v_texture_coord;

}
