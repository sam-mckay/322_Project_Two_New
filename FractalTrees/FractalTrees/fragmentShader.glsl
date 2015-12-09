#version 420 core

in vec4 colorsExport;
in vec2 texCoordsExport;
in int objectIDExport;
uniform int switchOn;

uniform sampler2D grassTex;


out vec4 colorsOut;

void main(void)
{

	if (switchOn == 0)
	{
		vec4 fieldTexColor = texture(grassTex, texCoordsExport);
		colorsOut = colorsExport * fieldTexColor;
	}
	else if (objectIDExport == 1)
	{
		colorsOut = colorsExport;
	}
}