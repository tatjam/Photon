#version 330 core

in vec3 normal;
in vec3 wPos;

out vec4 color;

void main()
{
	vec3 fcolor = normal;
	color = vec4(fcolor , 1.0f);
}