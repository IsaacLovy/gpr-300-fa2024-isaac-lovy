#version 450
layout(location = 0) out vec4 FragColor;
in vec2 UV;

uniform layout(binding = 0) sampler2D _gWorldPos;
uniform layout(binding = 1) sampler2D _gWorldNormal;
uniform layout(binding = 2) sampler2D _gAlbedo;
uniform layout(binding = 3) sampler2D _ShadowMap;

void main()
{
	vec3 worldPos = texture(_gWorldPos, UV).xyz;
	vec3 albedo = texture(_gAlbedo, UV).rbg;
	vec3 normal = texture(_gWorldNormal, UV).xyz;

	vec3 lightColor = vec3(1.0,1.0,1.0);
	FragColor = vec4(albedo * lightColor, 1.0);
}