#version 450
layout(location = 0) out vec4 FragColor;
in vec2 UV;

layout(binding = 0) uniform sampler2D _gWorldPos;
layout(binding = 1) uniform sampler2D _gWorldNormal;
layout(binding = 2) uniform sampler2D _gAlbedo;
layout(binding = 3) uniform sampler2D _ShadowMap;

void main()
{
	vec3 worldPos = texture(_gWorldPos, UV).xyz;
	vec3 albedo = texture(_gAlbedo, UV).rbg;
	vec3 normal = texture(_gWorldNormal, UV).xyz;

	//vec3 lightColor = vec3(1.0,1.0,1.0);
	FragColor = vec4(albedo, 1.0);
}