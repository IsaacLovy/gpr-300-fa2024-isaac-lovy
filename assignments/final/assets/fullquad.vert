#version 450 core
layout (location = 0) out vec2 UV;

void main() {
	float u = float(((uint(gl_VertexID)+2u) / 3u) % 2u);
	float v = float(((uint(gl_VertexID)+1u) / 3u) % 2u);
	UV = vec2(u, v);
	gl_Position = vec4(-1.0+u*2.0, -1.0+v*2.0, 0.0, 1.0);
}