#version 450 core
layout (location = 0) in vec3 vPos;

out vec3 TexCoords;

uniform mat4 _ViewProjection;

uniform mat4 _Model;

void main()
{
	TexCoords = vPos;
	gl_Position = _ViewProjection * _Model * vec4(vPos,1.0);
}