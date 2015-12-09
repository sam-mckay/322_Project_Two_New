#version 420 core

layout(location=0) in vec4 squareCoords;
layout(location=1) in vec4 squareColors;

uniform mat4 projMat;
uniform mat4 modelViewMat;
uniform vec4 globAmb;

out vec4 colorsExport;
struct Material
{
	vec4 ambRefl;
	vec4 diffRefl;
	vec4 specefl;
	vec4 emitCols;
	float shininess;
};
uniform Material terrainFandB;


void main(void)
{
   //gl_Position = projMat * modelViewMat * squareCoords;
   //colorsExport = squareColors;

	gl_Position = projMat * modelViewMat * squareCoords;
	colorsExport = globAmb*terrainFandB.ambRefl;
}