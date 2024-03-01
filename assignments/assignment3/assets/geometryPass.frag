#version 450

layout(location = 0) out vec3 gWorldPos;
layout(location = 1) out vec3 gWorldNormal;
layout(location = 2) out vec3 gAlbedo;

in Surface{
	vec3 WorldPos; //Vertex position in world space
	vec3 WorldNormal; //Vertex normal in world space
	vec2 TexCoord;
	mat3 TBN;
}fs_in;

uniform sampler2D _MainTex;

void main(){
	gWorldPos = fs_in.WorldPos;
	//Or read from normal map + convert to worldspace
	gWorldNormal = normalize(fs_in.WorldNormal);
	gAlbedo = texture(_MainTex,fs_in.TexCoord).rgb;
}
