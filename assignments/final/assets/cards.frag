#version 450 core
out vec4 FragColor;

in Surface{
	vec3 WorldPos; //Vertex position in world space
	vec3 WorldNormal; //Vertex normal in world space
	vec2 TexCoord;
	mat3 TBN;
	float cardID;
}fs_in;


void main() 
{
	FragColor = vec4(fs_in.cardID, fs_in.cardID, fs_in.cardID, 1.0);
}