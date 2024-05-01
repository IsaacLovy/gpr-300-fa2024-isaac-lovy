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

//Color
uniform vec3		_StartColor				= vec3(1.0);
uniform vec3		_EndColor				= vec3(1.0);

//Flipbook
uniform float		_FlipbookColumns		= 8.0;
uniform float		_FlipbookRows			= 4.0;
uniform float		_FlipbookSpeed			= 1.0;
uniform bool		_MatchParticlePhase		= true;

//Forces
uniform vec3		_Gravity				= vec3(1.0);
uniform vec3		_Wind					= vec3(1.0);

//Position and Rotation
uniform vec3		_EmitterDimensions		= vec3(1.0);
uniform float		_RotationSpeed			= 80.0;
uniform float		_Rotation				= 0.0;
uniform bool		_RotationRandomOffset	= true;
uniform bool		_RotationRandomDirection= true;

//Movement
uniform float		_ParticleSpeed			= 1.0;
uniform vec3		_ParticleDirection		= vec3(0,1.0,0);
uniform float		_ParticleSpread			= 0.0;
uniform float		_ParticleVelocityStart	= 1.0;
uniform float		_ParticleVelocityEnd	= 1.0;

//Opacity
uniform float		_Opacity				= 4.0;
uniform float		_FadeInPower			= 0.5;
uniform float		_FadeOutPower			= 2.0;
uniform bool		_SoftEdges				= false;
uniform float		_AlphaClipThreshold		= 100.0;

//Scale
uniform float		_ParticleStartSize		= 1.0;
uniform float		_ParticleEndSize		= 1.0;

//DEBUG
uniform float		_DebugTime				= 0;
uniform float		_ManualTime				= 0;

//To Fragment Shader
out float lifetime;

out Surface{
	vec3 WorldPos;		//Vertex position in world space
	vec2 UV;
	vec3 vertColor;
}vs_out;

void main(){
	//Lifetime
	lifetime = _Time * _ParticleSpeed;
	lifetime = mix(lifetime, 1, 1);
	lifetime += vVertexColor;
	float flrLifetime = floor(lifetime);
	lifetime = fract(lifetime);
	
	//Random Values
	float randomColorSeed = vVertexColor * 255;
	randomColorSeed += 0.5;
	randomColorSeed = floor(randomColorSeed);
	randomColorSeed += flrLifetime;
	vec3 randomColor = noiseGen(randomColorSeed);
	vs_out.vertColor = randomColor;

	//Forces
	/*vec3 forces = _Wind + _Gravity;
	forces += vPos;
	forces *= _Model;
	forces *= lifetime;

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
	direction += forces;
	direction *= velocityLifetime;*/

	//Position Offset
	//vec3 position = randomColor;
	vec3 position = vPos;
	//position -= 0.5;
	//position *= _EmitterDimensions;
	//position += direction;
	//position *= -_Model;
	//position += billboard;
	vs_out.WorldPos = position;

	//Billboarding
	float billboardLerp = lifetime;
	float billboardScale = mix(_ParticleStartSize,_ParticleEndSize,billboardLerp);
	vec3  billboard = billboarding(vec3(0.0),vPos, vec2(billboardScale, billboardScale), _View,_ViewProjection, _Model);
	position = billboard;

	//Flipbook
	float flipSpeed = _Time * _FlipbookSpeed;
	flipSpeed = fract(flipSpeed);
	if(_MatchParticlePhase)
	{
		flipSpeed = lifetime;
	}
	vec2 UV = flipbook(vTexCoord, _FlipbookColumns,_FlipbookRows, 1.0);

	//Rotation
	/*float rotSpeed = _Time * _RotationSpeed;
	rotSpeed += _Rotation;
	if(_RotationRandomOffset){
		rotSpeed += directionRandOffset.y * 180;	
	}
	if(_RotationRandomDirection)
	{
		rotSpeed *= sign(directionRandOffset.x);
	}
	rotSpeed = radians(rotSpeed);
	vs_out.UV = rotator(UV, vec2(0.5,0.5), rotSpeed);*/

	//World Position Offset
	//gl_Position = _ViewProjection * _Model * vec4(1.0);
	gl_Position = _ViewProjection * _Model * vec4(position,1.0);
}