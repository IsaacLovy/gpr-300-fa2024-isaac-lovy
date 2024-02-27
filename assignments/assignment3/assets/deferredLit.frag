#version 450
layout(location = 0) out vec4 FragColor;
in vec2 UV;

uniform sampler2D _gWorldPos;
uniform sampler2D _gWorldNormal;
uniform sampler2D _gAlbedo;

struct Material {
	float Ka; //Ambient coefficient (0-1)
	float Kd; //Diffuse coefficient (0-1)
	float Ks; //Specular coefficient (0-1)
	float Shininess; //Affects size of specular highlight
};
uniform Material _Material;

uniform sampler2D _MainTex; 
uniform sampler2D _ShadowMap;

in vec4 LightSpacePos;

uniform float _MaxBias;
uniform float _MinBias;

uniform vec3 _EyePos;
uniform vec3 _LightDirection = vec3(0.0,-1.0,0.0);
uniform vec3 _LightColor = vec3(1.0);
uniform vec3 _AmbientColor = vec3(0.2,0.2,0.2);

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
			vec2 uv = sampleCoord.xy + vec2(x* texelOffset.x, y * texelOffset.y);
			totalShadow += step(texture(_ShadowMap, uv).r, myDepth);
		}
	}

	totalShadow /= 9.0;
	
	return totalShadow;
}

void main()
{
	vec3 worldPos = texture(_gWorldPos,UV).rgb;
	vec3 albedo = texture(_gAlbedo, UV).rgb;
	vec3 normal = texture(_gWorldNormal,UV).rgb;

	vec3 toLight = -_LightDirection;
	float diffuseFactor = max(dot(normal,toLight),0.0);
	//Calculate specularly reflected light
	vec3 toEye = normalize(_EyePos - worldPos);
	//Blinn-phong uses half angle
	vec3 h = normalize(toLight + toEye);
	float specularFactor = pow(max(dot(normal,h),0.0),_Material.Shininess);
	//Combination of specular and diffuse reflection
	float shadow = calcShadow(_ShadowMap, LightSpacePos, normal);
	vec3 lightColor = (_Material.Kd * diffuseFactor + _Material.Ks * specularFactor) * _LightColor * (1.0 - shadow);
	//Add in ambient color after the shadow effected lighting
	lightColor += _AmbientColor * _Material.Ka;
	FragColor = vec4(albedo * lightColor,1.0);
}