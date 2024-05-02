#version 450
out vec4 FragColor;		//The color of this fragment

in Surface{
	vec3 WorldPos;		//Vertex position in world space
	vec2 UV;
	vec3 vertColor;
	float lifetime;
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

//Opacity
uniform float		_Opacity				= 4.0;
uniform float		_FadeInPower			= 0.5;
uniform float		_FadeOutPower			= 2.0;
uniform bool		_SoftEdges				= false;
uniform float		_AlphaClipThreshold		= 100.0;

void main(){
	FragColor = vec4(fs_in.vertColor,1.0);
}