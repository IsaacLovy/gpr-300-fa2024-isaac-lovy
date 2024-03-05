#version 450
layout(location = 0) out vec4 FragColor;
in vec2 UV;

layout(binding = 0) uniform sampler2D _gWorldPos;
layout(binding = 1) uniform sampler2D _gWorldNormal;
layout(binding = 2) uniform sampler2D _gAlbedo;
layout(binding = 3) uniform sampler2D _ShadowMap;

uniform vec3 _EyePos;
uniform vec4 _LightViewProj;
uniform vec3 _LightDirection = vec3(0.0,-1.0,0.0);
uniform vec3 _LightColor = vec3(1.0);
uniform vec3 _AmbientColor = vec3(0.2,0.2,0.2);

uniform float _MaxBias;
uniform float _MinBias;

float calcShadow(sampler2D shadowMap, vec4 lightSpacePos, vec3 worldnormal)
{

	vec3 sampleCoord = lightSpacePos.xyz / lightSpacePos.w;
	sampleCoord = sampleCoord * 0.5 + 0.5;
	float bias = max(_MaxBias * (1.0 - dot(worldnormal, -_LightDirection)), _MinBias);
	float myDepth = sampleCoord.z - bias;

	if (myDepth > 1) return 0;

	float totalShadow = 0;

	vec2 texelOffset = 1.0 / textureSize(_ShadowMap, 0);

	for(int y = -1; y<=1; y++)
	{
		for(int x = -1; x<=1; x++)
		{
			vec2 uv = sampleCoord.xy + vec2(x* texelOffset.x, y * texelOffset.y);
			totalShadow += step(texture(_ShadowMap, uv).r, myDepth);
		}
	}

	totalShadow /= 9.0;
	
	return totalShadow;
}

vec3 calcLighting(vec3 worldPos, vec3 normal)
{

	vec4 lightSpacePos = _LightViewProj * vec4(worldPos, 1.0);
	
	float shadow = calcShadow(_ShadowMap, lightSpacePos, normal);
	
	vec3 lightColor = _LightColor * (1.0 - shadow);

	return lightColor;
}

void main()
{
	vec3 worldPos = texture(_gWorldPos, UV).xyz;
	vec3 albedo = texture(_gAlbedo, UV).rgb;
	vec3 normal = texture(_gWorldNormal, UV).xyz;

	vec3 lightColor = calcLighting(worldPos, normal);
	FragColor = vec4(albedo * lightColor, 1.0);
}