#version 420 core

layout(location=0) in vec4 squareCoords;
layout(location=1) in vec4 squareColors;

layout(location=0) in vec4 terrainCoords;
layout(location=1) in vec3 terrainNormals;

uniform mat4 projMat;
uniform mat4 modelViewMat;
uniform vec4 globAmb;
uniform mat3 normalMat;

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

struct Light
{
	vec4 ambCols;
	vec4 difCols;
	vec4 specCols;
	vec4 coords;
}
uniform Light light0;


void main(void)
{
   //gl_Position = projMat * modelViewMat * squareCoords;
   //colorsExport = squareColors;

	gl_Position = projMat * modelViewMat * squareCoords;
	colorsExport = globAmb*terrainFandB.ambRefl;

	normal = normalize(normalMat * terrainNormals);
	lightDirection = normalize(vec3(light0.coords));
	colorsExport = max(dot(normal, lightDirection), 0.0f) * (light0.difCols * terrainFandB.difRefl);
}