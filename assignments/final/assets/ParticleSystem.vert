#version 450

#include particleFuncsCore.vert

//Vertex attributes
layout(location = 0) in vec3	vPos;
layout(location = 1) in vec3	vNormal;
layout(location = 2) in vec2	vTexCoord;
layout(location = 3) in vec3	vTangent;
layout(location = 4) in float	vVertexColor;

uniform float _Time;
uniform mat4 _Model; 
uniform mat4 _View;
uniform mat4 _ViewProjection;
uniform mat4 _LightViewProj;

//Flipbook
uniform float		_FlipbookColumns		= 8.0;
uniform float		_FlipbookRows			= 4.0;
uniform float		_FlipbookSpeed			= 25.0;
uniform bool		_MatchParticlePhase		= false;

//Forces
uniform vec3		_Gravity				= vec3(0, -600.0, 0);
uniform vec3		_Wind					= vec3(0.0);

//Position and Rotation
uniform vec3		_EmitterDimensions		= vec3(10.0);
uniform float		_RotationSpeed			= 80.0;
uniform float		_Rotation				= 0.0;
uniform bool		_RotationRandomOffset	= true;
uniform bool		_RotationRandomDirection= true;

//Movement
uniform float		_ParticleSpeed			= 0.3;
uniform vec3		_ParticleDirectionMin	= vec3(0,1,0);
uniform vec3		_ParticleDirectionMax	= vec3(0,1,0);
uniform float		_ParticleSpread			= 15.0;
uniform float		_ParticleVelocityStart	= 600.0;
uniform float		_ParticleVelocityEnd	= 600.0;

//Scale
uniform float		_ParticleStartSize		= 0.03;
uniform float		_ParticleEndSize		= 0.9;

//DEBUG
uniform float		_DebugTime				= 0;
uniform float		_ManualTime				= 0;

//To Fragment Shader
out float lifetime;

out Surface{
	vec3 WorldPos;		//Vertex position in world space
	vec2 UV;
	vec3 vertColor;
	float lifetime;
}vs_out;

void main(){
	//Lifetime
	lifetime = _Time * _ParticleSpeed;
	//lifetime = mix(lifetime, _ManualTime, _DebugTime);
	lifetime += vVertexColor;
	float flrLifetime = floor(lifetime);
	lifetime = fract(lifetime);
	
	//Random Values
	float randomColorSeed = vVertexColor * 255;
	randomColorSeed += 0.5;
	randomColorSeed = floor(randomColorSeed);
	randomColorSeed += flrLifetime;
	vec3 randomColor = noiseGen(vec3(randomColorSeed, randomColorSeed+100,randomColorSeed+500));
	vs_out.vertColor = randomColor;

	//Forces
	vec3 forces = _Wind + _Gravity;
	//forces += vPos;
	//forces *= _Model;
	forces *= lifetime;

	//Direction
	vec3 direction = vec3(remapFloat(_ParticleSpread,0,360,0,2));
	vec3 directionRandOffset = randomColor;
	directionRandOffset *= 2;
	directionRandOffset -= 1;
	direction *= directionRandOffset;

	//Particle Direction Range
	vec3 particleDirection = mix(_ParticleDirectionMin, _ParticleDirectionMax, randomColor.r);

	direction += particleDirection;
	direction = normalize(direction);
	float velocityLifetime = lifetime;
	float velocityLerp = mix(_ParticleVelocityStart, _ParticleVelocityEnd, velocityLifetime);
	direction *= velocityLerp;
	direction += forces;
	direction *= velocityLifetime;

	//Position Offset
	//vec3 position = randomColor;
	//vec3 position = vPos;
	//position -= 0.5;
	//position *= _EmitterDimensions;
	//position += direction;
	//position *= -_Model;
	//position += billboard;


	//Rotation
	float rotSpeed = _Time * _RotationSpeed;
	rotSpeed += _Rotation;
	if(_RotationRandomOffset){
		rotSpeed += directionRandOffset.y * 180;	
	}
	if(_RotationRandomDirection)
	{
		rotSpeed *= sign(directionRandOffset.x);
	}
	rotSpeed = radians(rotSpeed);

	//Billboarding
	float billboardLerp = lifetime;
	float billboardScale = mix(_ParticleStartSize,_ParticleEndSize,billboardLerp);
	vec3 billboard = billboarding(direction, vPos, vec2(billboardScale, billboardScale), _View, rotSpeed);
	vec3 position = billboard;

	vec2 rotUV = rotator(vTexCoord, vec2(0.5,0.5), rotSpeed);

	//Flipbook
	float flipSpeed = _Time * _FlipbookSpeed;
	flipSpeed = floor(flipSpeed);
	if(_MatchParticlePhase)
	{
		flipSpeed = floor(lifetime);
	}
	vec2 UV = flipbook(vTexCoord, _FlipbookColumns,_FlipbookRows, flipSpeed, vec2(0));

	//World Position Offset
	//gl_Position = _ViewProjection * _Model * vec4(1.0);
	vs_out.UV = UV;
	vs_out.WorldPos = position;
	vs_out.lifetime = lifetime;
	gl_Position = _ViewProjection * _Model * vec4(position, 1.0);
}