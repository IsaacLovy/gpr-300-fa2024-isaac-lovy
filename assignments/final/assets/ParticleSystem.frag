#version 450
out vec4 FragColor;		//The color of this fragment
in Surface{
	vec3 WorldPos;		//Vertex position in world space
	vec3 WorldNormal;	//Vertex normal in world space
	vec2 TexCoord;
	mat3 TBN;
}fs_in;

struct Material {
	float Ka;			//Ambient coefficient (0-1)
	float Kd;			//Diffuse coefficient (0-1)
	float Ks;			//Specular coefficient (0-1)
	float Shininess;	//Affects size of specular highlight
};
uniform Material _Material;

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

uniform vec3 _EyePos;
uniform vec3 _LightDirection = vec3(0.0,-1.0,0.0);
uniform vec3 _LightColor = vec3(1.0);
uniform vec3 _AmbientColor = vec3(0.2,0.2,0.2);

uniform sampler2D _NormalTex;


void main(){
	FragColor = vec4(1.0);
}