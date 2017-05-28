#version 330 core
out vec4 FragColor;
in vec2 TexCoords;

uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gAlbedo;



uniform int renderMode;

struct DirLight
{
	vec3 dir;
	vec3 col;
};

struct PointLight
{
	vec3 pos;
	vec3 col;

	float linear;
	float quadratic;
};




// Lights: (64 is an arbitrary number, it's the max limit)
const int MAX_PLIGHTS = 64;

// Light count in the scene
uniform int pLightCount;

uniform PointLight pointLights[MAX_PLIGHTS];

uniform DirLight dirLight;

uniform vec3 ambient;

uniform vec3 viewPos;

vec4 finalPass()
{
	vec3 FragPos = texture(gPosition, TexCoords).rgb;
	vec3 Normal = texture(gNormal, TexCoords).rgb;
	vec3 Diffuse = texture(gAlbedo, TexCoords).rgb;
	float Specular = texture(gAlbedo, TexCoords).a;

	vec3 lighting = Diffuse * ambient; 
	vec3 viewDir = normalize(viewPos - FragPos);

	for (int i = 0; i < pLightCount; ++i)
	{
		// Diffuse
		vec3 lightDir = normalize(pointLights[i].pos - FragPos);
		vec3 diffuse = max(dot(Normal, lightDir), 0.0) * Diffuse * pointLights[i].col;
		// Specular
		vec3 halfwayDir = normalize(lightDir + viewDir);
		float spec = pow(max(dot(Normal, halfwayDir), 0.0), 16.0);
		vec3 specular = pointLights[i].col * spec * Specular;
		// Attenuation
		float distance = length(pointLights[i].pos - FragPos);
		float attenuation = 1.0 / (1.0 + pointLights[i].linear * distance + pointLights[i].quadratic * distance * distance);
		diffuse *= attenuation;
		specular *= attenuation;
		lighting += diffuse + specular;
	}

	return vec4(lighting, 1.0f);
}

void main()
{
	if (renderMode == 0)
	{
		// Finished rendering
		FragColor = finalPass();
	}
	else if (renderMode == 1)
	{
		// Position Rendering
		FragColor = vec4(texture(gPosition, TexCoords).rgb, 1.0f);
	}
	else if (renderMode == 2)
	{
		// Normal Rendering
		FragColor = vec4(texture(gNormal, TexCoords).rgb, 1.0f);
	}
	else if (renderMode == 3)
	{
		// Albedo Rendering
		FragColor = vec4(texture(gAlbedo, TexCoords).rgb, 1.0f);
	}
	else
	{
		// Specular Rendering
		FragColor = vec4(vec3(texture(gAlbedo, TexCoords).a), 1.0f);
	}
	
}