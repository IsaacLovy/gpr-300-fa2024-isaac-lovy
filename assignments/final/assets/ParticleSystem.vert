#version 450
#include "particleFuncsCore.vert"
//Vertex attributes
layout(location = 0) in vec3	vPos;
layout(location = 1) in vec3	vNormal;
layout(location = 2) in vec2	vTexCoord;
layout(location = 3) in vec3	vTangent;
layout(location = 4) in float	vVertexColor;

uniform float _Time;
uniform mat4 _Model; 
uniform mat4 _ViewProjection;
uniform mat4 _LightViewProj;

//Color
uniform vec3		_StartColor				= vec3(1.0);
uniform vec3		_EndColor				= vec3(1.0);

//Flipbook
uniform sampler2D	_FlipbookTexture;
uniform float		_FlipbookColumns		= 8.0;
uniform float		_FlipbookRows			= 4.0;

//Forces
uniform vec3		_Gravity				= vec3(1.0);
uniform vec3		_Wind					= vec3(1.0);

//Position
uniform vec3		_EmitterDimensions		= vec3(1.0);

//Movement
uniform float		_ParticleSpeed			= 1;
uniform vec3		_ParticleDirection		= vec3(0,1.0,0);
uniform float		_ParticleSpread			= 0;
uniform float		_ParticleVelocityStart	= 1;
uniform float		_ParticleVelocityEnd	= 1;

//Opacity
uniform float		_Opacity				= 4.0;
uniform float		_FadeInPower			= 0.5;
uniform float		_FadeOutPower			= 2.0;
uniform bool		_SoftEdges				= false;
uniform float		_AlphaClipThreshold		= 100;

//Scale
uniform float		_ParticleStartSize		= 0;
uniform float		_ParticleEndSize		= 0;

//DEBUG
uniform float		_DebugTime				= 0;
uniform float		_ManualTime				= 0;

out vec4 LightSpacePos;

out Surface{
	vec3 WorldPos;		//Vertex position in world space
	vec3 WorldNormal;	//Vertex normal in world space
	vec2 TexCoord;
	mat3 TBN;
}vs_out;

void main(){
	//Lifetime
	float lifetime = _Time * _ParticleSpeed;
	lifetime = mix(lifetime, _ManualTime, _DebugTime);
	lifetime += vVertexColor;
	float flrLifetime = floor(lifetime);
	lifetime = fract(lifetime);
	
	//Random Values
	float randomColorSeed = vVertexColor * 255;
	randomColorSeed += 0.5;
	randomColorSeed = floor(randomColorSeed);
	randomColorSeed += flrLifetime;
	vec3 randomColor = noise3(randomColorSeed);

	//Position Offset
	vec3 randomColorOffset = randomColor;
	randomColorOffset -= 0.5;
	randomColorOffset *= _EmitterDimensions;

	//Billboarding
	float billboardLerp = lifetime;
	float billboardScale = mix(_ParticleStartSize,_ParticleEndSize,billboardLerp);
	//Function needs to be implemented

	//Direction
	float direction = remapFloat(_ParticleSpread,0,360,0,2);
	vec3 directionRandOffset = randomColor;
	directionRandOffset *= 2;
	directionRandOffset -= 1;
	direction *= directionRandOffset;
	direction += _ParticleDirection;
	direction = normalize(direction);
	float velocityLifetime = lifetime;
	float velocityLerp = mix(_ParticleVelocityStart, _ParticleVelocityEnd,velocityLifetime);
	direction *= velocityLerp;
	//NEED TO FACTOR IN FORCES
	//direction +=
	direction *= velocityLifetime;

	LightSpacePos =  _LightViewProj * _Model * vec4(vPos,1.0);
	gl_Position = _ViewProjection * _Model * vec4(vPos,1.0);
}