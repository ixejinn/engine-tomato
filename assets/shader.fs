#version 450 core

out vec4 FragColor;

in vec2 vTexCoord;

uniform sampler2D uTexture;
uniform vec4 uColor;

void main()
{
	FragColor = texture(uTexture, TexCoord) * uColor;
}