#version 330 core

in vec3 normal;
in vec3 wPos;

out vec4 color;

void main()
{
	color = vec4(reflect(normal, wPos) , 1.0f);
}