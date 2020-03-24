#version 430

layout(location = 0) in vec3 v_position;
layout(location = 1) in vec3 v_normal;
layout(location = 2) in vec2 v_texture_coord;
flat out float inv;	
out vec3 rot;

// Uniform properties
uniform mat4 Model;
uniform int shatter;
uniform float impactSpeed;
uniform float timeScale;

struct Particle
{
	vec4 position;
	vec4 speed;
	vec3 rot;
	float inv;
};

layout(std430, binding = 0) buffer particles {
	Particle data[];
};

void main()
{
	vec3 pos = data[gl_VertexID].position.xyz;
	vec3 spd = data[gl_VertexID].speed.xyz;
	vec3 aux = data[gl_VertexID].rot;
	
	if (shatter == 1 && pos.y > -5.0f)
	{
		float dt = 0.1 * timeScale;

		pos = pos + vec3(spd.x, spd.y, spd.z + impactSpeed) * dt + vec3(0, -0.9, 0) * dt * dt/2.0f;
		spd = spd + vec3(0, -0.9, 0) * dt;
		aux += vec3(spd.x, spd.y, spd.z + impactSpeed) * dt;

		data[gl_VertexID].position.xyz =  pos;
		data[gl_VertexID].speed.xyz =  spd;
		data[gl_VertexID].rot =  aux;
	}

	if (pos.y < -5.0f) {
		pos.y = -5.0f;
	}

	gl_Position = Model * vec4(pos, 1);
	inv = data[gl_VertexID].inv;
	rot = data[gl_VertexID].rot;
}


