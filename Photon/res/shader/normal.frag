#version 330 core
layout(location = 0) out vec3 gPosition;
layout(location = 1) out vec3 gNormal;
layout(location = 2) out vec4 gAlbedo;

in vec2 TexCoords;
in vec3 FragPos;
in vec3 Normal;

void main()
{
	// Store the fragment position vector in the first gbuffer texture
	gPosition = FragPos;
	// Also store the per-fragment normals into the gbuffer
	gNormal = normalize(Normal);
	// And the diffuse per-fragment color
	gAlbedo.rgb = Normal;
	// Store specular intensity in gAlbedoSpec's alpha component
	gAlbedo.a = 0.0f;
}