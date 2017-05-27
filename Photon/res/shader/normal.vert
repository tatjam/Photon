#version 330 core

layout(location = 0) in vec3 pos;
layout(location = 1) in vec3 nor;
layout(location = 2) in vec2 tex;

out vec3 normal;
out vec3 wPos;

uniform mat4 world;
uniform mat4 view;
uniform mat4 proj;

void main()
{
	gl_Position = proj * view * world * vec4(pos, 1.0f);
	//gl_Position = vec4(pos, 1.0f);
	normal = nor;
	wPos = (world * vec4(pos, 1.0f)).xyz;
}
