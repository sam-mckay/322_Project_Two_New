#version 420 core

in vec4 colorsExport;
in vec2 texCoordsExport;

uniform sampler2D grassTex;


out vec4 colorsOut;

void main(void)
{
	vec4 fieldTexColor = texture(grassTex, texCoordsExport);
	colorsOut = colorsExport*fieldTexColor;
}