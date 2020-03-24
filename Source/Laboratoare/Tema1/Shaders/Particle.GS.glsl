#version 430
layout(points) in;
layout(triangle_strip, max_vertices = 18) out;
in float inv [];
in vec3 rot[];

uniform mat4 View;
uniform mat4 Projection;
uniform vec3 eye_position;
uniform int shatter;
uniform float triangleSize;

layout(location = 1) out vec3 vn;
layout(location = 2) out vec3 wp;
layout(location = 3) out vec3 wn;

vec3 vpos = gl_in[0].gl_Position.xyz;

mat4 rotationX(in float angle) {
	return mat4(1.0,		0,			0,			0,
			 	0, 	cos(angle),	-sin(angle),		0,
				0, 	sin(angle),	 cos(angle),		0,
				0, 			0,			  0, 		1);
}

mat4 rotationY(in float angle) {
	return mat4(cos(angle),		0,		sin(angle),	0,
			 	0,				1.0,			 0,	0,
				-sin(angle),	0,		cos(angle),	0,
				0, 				0,				0,	1);
}

mat4 rotationZ(in float angle) {
	return mat4(cos(angle),		-sin(angle),	0,	0,
			 	sin(angle),		cos(angle),		0,	0,
				0,				0,				1,	0,
				0,				0,				0,	1);
}


void EmitPoint(vec4 offset, vec3 normal)
{
	if (shatter == 1) {
		offset = rotationZ(rot[0].z) * rotationY(rot[0].y) * rotationX(rot[0].x) * offset;
		vec3 aux = normalize((rotationZ(rot[0].z) * rotationY(rot[0].y) * rotationX(rot[0].x) * vec4(normal, 1)).xyz);
		wn = aux;
		if (aux.x < 0) {
			aux.x = 0;
		}
		if (aux.y < 0) {
			aux.y = 0;
		}
		if (aux.z < 0) {
			aux.z = 0;
		}
		vn = aux;
		wp = vpos + offset.xyz;
		gl_Position = Projection * View * vec4(wp, 1);
	}
	else {
		vec3 aux = normal;
		wn = aux;
		if (aux.x < 0) {
			aux.x = 0;
		}
		if (aux.y < 0) {
			aux.y = 0;
		}
		if (aux.z < 0) {
			aux.z = 0;
		}
		vn = aux;
		wp = vpos + offset.xyz;
		gl_Position = Projection * View * vec4(wp, 1);
	}
	EmitVertex();
}

void main()
{
	float ds = triangleSize;
	float ds2 = -triangleSize / 4;

	if (inv[0] == 0){
		EmitPoint(vec4(-ds, -ds, 0, 1), vec3(0, 0, 1));
		EmitPoint(vec4(ds, -ds, 0, 1), vec3(0, 0, 1));
		EmitPoint(vec4(-ds, ds, 0, 1), vec3(0, 0, 1));

		EndPrimitive(); // first triangle

		EmitPoint(vec4(-ds, -ds, ds2, 1), vec3(0, 0, -1));
		EmitPoint(vec4(ds, -ds, ds2, 1), vec3(0, 0, -1));
		EmitPoint(vec4(-ds, ds, ds2, 1), vec3(0, 0, -1));

		EndPrimitive(); // second triangle
		
		EmitPoint(vec4(-ds, -ds, ds2, 1), vec3(-1, 0, 0));
		EmitPoint(vec4(-ds, -ds, 0, 1), vec3(-1, 0, 0));
		EmitPoint(vec4(-ds, ds, ds2, 1), vec3(-1, 0, 0));
		EmitPoint(vec4(-ds, ds, 0, 1), vec3(-1, 0, 0));

		EndPrimitive(); // first rectangle

		EmitPoint(vec4(-ds, -ds, 0, 1), vec3(0, -1, 0));
		EmitPoint(vec4(-ds, -ds, ds2, 1), vec3(0, -1, 0));
		EmitPoint(vec4(ds, -ds, 0, 1), vec3(0, -1, 0));
		EmitPoint(vec4(ds, -ds, ds2, 1), vec3(0, -1, 0));

		EndPrimitive(); // second rectangle

		EmitPoint(vec4(ds, -ds, 0, 1), normalize(vec3(1, 1, 0)));
		EmitPoint(vec4(ds, -ds, ds2, 1), normalize(vec3(1, 1, 0)));
		EmitPoint(vec4(-ds, ds, 0, 1), normalize(vec3(1, 1, 0)));
		EmitPoint(vec4(-ds, ds, ds2, 1), normalize(vec3(1, 1, 0)));

		EndPrimitive(); // third rectangle
	} else {
		EmitPoint(vec4(ds, ds, 0, 1), vec3(0, 0, 1));
		EmitPoint(vec4(ds, -ds, 0, 1), vec3(0, 0, 1));
		EmitPoint(vec4(-ds, ds, 0, 1), vec3(0, 0, 1));

		EndPrimitive();

		EmitPoint(vec4(ds, ds, ds2, 1), vec3(0, 0, -1));
		EmitPoint(vec4(ds, -ds, ds2, 1), vec3(0, 0, -1));
		EmitPoint(vec4(-ds, ds, ds2, 1), vec3(0, 0, -1));

		EndPrimitive();
		
		EmitPoint(vec4(ds, -ds, 0, 1), vec3(1, 0, 0));
		EmitPoint(vec4(ds, -ds, ds2, 1), vec3(1, 0, 0));
		EmitPoint(vec4(ds, ds, 0, 1), vec3(1, 0, 0));
		EmitPoint(vec4(ds, ds, ds2, 1), vec3(1, 0, 0));

		EndPrimitive(); // first rectangle

		EmitPoint(vec4(ds, ds, ds2, 1), vec3(0, 1, 0));
		EmitPoint(vec4(ds, ds, 0, 1), vec3(0, 1, 0));
		EmitPoint(vec4(-ds, ds, ds2, 1), vec3(0, 1, 0));
		EmitPoint(vec4(-ds, ds, 0, 1), vec3(0, 1, 0));

		EndPrimitive(); // second rectangle

		EmitPoint(vec4(-ds, ds, 0, 1), normalize(vec3(-1, -1, 0)));
		EmitPoint(vec4(-ds, ds, ds2, 1), normalize(vec3(-1, -1, 0)));
		EmitPoint(vec4(ds, -ds, 0, 1), normalize(vec3(-1, -1, 0)));
		EmitPoint(vec4(ds, -ds, ds2, 1), normalize(vec3(-1, -1, 0)));

		EndPrimitive(); // second rectangle
	}
}
