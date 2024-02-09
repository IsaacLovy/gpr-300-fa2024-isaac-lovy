#version 450
out vec4 FragColor;
in vec2 UV;

uniform float _Vignette_I = 15;
uniform float _Vignette_D = 0.25;
uniform float _RedOffset = 0;
uniform float _GreenOffset = 0;
uniform float _BlueOffset = 0;

uniform sampler2D _ColorBuffer;
void main(){

	vec3 color = texture(_ColorBuffer,UV).rgb;

	//Chromatic Aberation
	float r = texture(_ColorBuffer,UV - _RedOffset).r;
	float g = texture(_ColorBuffer,UV - _GreenOffset).g;
	float b = texture(_ColorBuffer,UV - _BlueOffset).b;
	color.rgb = vec3(r, g, b);
	
	//vignette effect
	//https://www.shadertoy.com/view/lsKSWR
	vec2 vig_uv = UV * (1.0 - UV.xy);
	float vig = vig_uv.x*vig_uv.y*_Vignette_I;
	vig = pow(vig, _Vignette_D);
	color.rgb *= vig;

	FragColor = vec4(color, 1.0);
}
