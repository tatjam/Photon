#version 330 core
out vec4 color;
in vec2 TexCoords;

uniform sampler2D hdrBuffer;


// Do nothing at all :P
void main()
{
	vec3 hdrColor = texture(hdrBuffer, TexCoords).rgb;
	color = vec4(hdrColor, 1.0f);
}