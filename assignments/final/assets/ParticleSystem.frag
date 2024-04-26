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

//Flipbook
uniform sampler2D	_FlipbookTexture;

void main(){
	FragColor = vec4(1.0);
}