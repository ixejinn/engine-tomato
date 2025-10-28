#version 330 core

out vec4 FragColor;
in vec2 TexCoord;

uniform sampler2D uTexture;
uniform vec4 uColor;

void main()
{
	FragColor = texture(uTexture, TexCoord) * uColor;
	//FragColor = uColor;
}