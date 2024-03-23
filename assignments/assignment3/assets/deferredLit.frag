#version 450
layout(location = 0) out vec4 FragColor;
in vec2 UV;

layout(binding = 0) uniform sampler2D _gWorldPos;
layout(binding = 1) uniform sampler2D _gWorldNormal;
layout(binding = 2) uniform sampler2D _gAlbedo;
layout(binding = 3) uniform sampler2D _ShadowMap;

uniform vec3 _EyePos;
uniform vec4 _LightViewProj;
uniform vec3 _LightDirection;
uniform vec3 _AmbientColor = vec3(0.2,0.2,0.2);
uniform vec3 _DirLightColor = vec3(0.5, 0.5, 0.5);

uniform float _PointIntensity = 1.0;

uniform float _MaxBias;
uniform float _MinBias;

struct Material {
	float Ka; //Ambient coefficient (0-1)
	float Kd; //Diffuse coefficient (0-1)
	float Ks; //Specular coefficient (0-1)
	float Shininess; //Affects size of specular highlight
};
uniform Material _Material;

struct PointLight{
	vec3 position;
	float radius;
	vec4 color;
};

#define MAX_POINT_LIGHTS 64

uniform PointLight _PointLights[MAX_POINT_LIGHTS];

float attenuateExponential(float dist, float radius){
	float i = clamp(1.0 - pow(dist/radius,4.0),0.0,1.0);
	return i * i;	
}

vec3 calcPointLight(PointLight light, vec3 normal, vec3 pos){
	vec3 diff = light.position - pos;
	vec3 toLight = normalize(diff);
	vec3 toEye = normalize(pos - _EyePos);
	float diffuseFactor = max(dot(normal,toLight),0.0);
	vec3 h = normalize(toLight + toEye);
	float specularFactor = pow(max(dot(normal,h),0.0),_Material.Shininess);
	vec4 lightColor = (_Material.Kd * diffuseFactor + _Material.Ks * specularFactor) * light.color;
	float d = length(diff);
	lightColor *= attenuateExponential(d, light.radius);
	return lightColor.rgb;
}

float calcShadow(sampler2D shadowMap, vec4 lightSpacePos, vec3 worldNormal)
{
	vec3 sampleCoord = lightSpacePos.xyz / lightSpacePos.w;
	sampleCoord = sampleCoord * 0.5 + 0.5;
	float bias = max(_MaxBias * (1.0 - dot(worldNormal, -_LightDirection)), _MinBias);
	float myDepth = sampleCoord.z - bias;

	if (myDepth > 1) return 0;

	float totalShadow = 0;

	vec2 texelOffset = 1.0 / textureSize(_ShadowMap, 0);

	for(int y = -1; y<=1; y++)
	{
		for(int x = -1; x<=1; x++)
		{
			vec2 uv = sampleCoord.xy + vec2(x * texelOffset.x, y * texelOffset.y);
			totalShadow += step(texture(_ShadowMap, uv).r, myDepth);
		}
	}

	totalShadow /= 9.0;
	
	return totalShadow;
}

vec3 calcDirLighting(vec3 worldPos, vec3 normal)
{
	vec4 lightSpacePos = _LightViewProj * vec4(worldPos, 1.0);
	
	float shadow = calcShadow(_ShadowMap, lightSpacePos, normal);
	
	vec3 toLight = -_LightDirection;

	float diffuseFactor = max(dot(normal,toLight),0.0);
	vec3 toEye = normalize(_EyePos - worldPos);
	vec3 h = normalize(toLight + toEye);
	float specularFactor = pow(max(dot(normal,h),0.0),_Material.Shininess);
	
	vec3 lightColor = (_Material.Kd * diffuseFactor + _Material.Ks * specularFactor) * _DirLightColor * (1.0 - shadow);

	return lightColor;
}

void main()
{
	vec3 worldPos = texture(_gWorldPos, UV).rgb;
	vec3 normal = texture(_gWorldNormal, UV).rgb;
	vec3 albedo = texture(_gAlbedo, UV).rgb;

	vec3 totalLight = vec3(0);

	totalLight += calcDirLighting(worldPos, normal);

	for (int i = 0; i < MAX_POINT_LIGHTS; i ++)
	{
		totalLight+=calcPointLight(_PointLights[i], normal, worldPos) * _PointIntensity;
	}

	FragColor = vec4(albedo * totalLight,0);
}