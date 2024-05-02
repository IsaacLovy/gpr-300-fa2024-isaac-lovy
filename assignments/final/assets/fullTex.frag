#version 450
out vec4 FragColor;
in vec2 UV;

uniform sampler2D _ColorBuffer;

void main(){

	vec4 color = texture(_ColorBuffer,UV);

	FragColor = color;
}
