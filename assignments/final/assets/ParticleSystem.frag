#version 450
out vec4 FragColor;		//The color of this fragment
#include particleFuncs.frag
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
uniform vec4		_StartColor				= vec4(0.865,1.0,1.0,1.0);
uniform vec4		_EndColor				= vec4(0.514,1.0,1.0,1.0);

//Flipbook
uniform sampler2D	_FlipbookTexture;

//Opacity
uniform float		_Opacity				= 4.0;
uniform float		_FadeInPower			= 0.5;
uniform float		_FadeOutPower			= 2.0;
uniform float		_AlphaClipThreshold		= 0.04;

void main(){
	//Opacity
	float opacityLifetime = fs_in.lifetime;
	float fadeInPower = pow(opacityLifetime,_FadeInPower);
	opacityLifetime = 1 - opacityLifetime;
	opacityLifetime = pow(opacityLifetime,_FadeOutPower);
	opacityLifetime *= fadeInPower;
	opacityLifetime *= _Opacity;
	float opacity = clamp(opacityLifetime,0.0,1.0);

	//Color
	vec3 colorLerp = mix(_EndColor.xyz, _StartColor.xyz, opacity);
	float opacityLerp = mix(_EndColor.w, _StartColor.w, opacity);

	//Alpha Clip
	vec4 color = texture(_FlipbookTexture,fs_in.UV);
	float a = color.w;
	a = clipPixel(a,_AlphaClipThreshold);
	a *= opacity;
	a *= opacityLerp;
	color *= vec4(colorLerp,1.0);

	//Output
	FragColor = vec4(color.xyz,a);
}