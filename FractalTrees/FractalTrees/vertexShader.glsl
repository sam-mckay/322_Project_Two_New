#version 420 core

///layout(location=0) in vec4 squareCoords;
//layout(location=1) in vec4 squareColors;
layout(location=0) in int objectID;
layout(location=1) in vec4 terrainCoords;
layout(location=2) in vec3 terrainNormals;
layout(location=3) in vec2 terrainTexCoords;
layout(location=4) in vec4 terrainColors;

layout(location = 5) in int treeObjectID;
layout(location = 6) in vec4 treeCoords;
layout(location = 7) in vec3 treeNormals;
layout(location = 8) in vec2 nullTreeCoords;
layout(location = 9) in vec4 treeColors;

uniform mat4 projMat;
uniform mat4 modelViewMat;
uniform mat4 translationMat;
uniform vec4 globAmb;
uniform mat3 normalMat;
uniform int switchOn;


out vec4 colorsExport;
out vec2 texCoordsExport;
out int objectIDExport;

struct Material
{
	vec4 ambRefl;
	vec4 difRefl;
	vec4 specRefl;
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
};
uniform Light light0;

vec3 normal;
vec3 lightDirection;

void main(void)
{

	if (switchOn == 0)
	{
		gl_Position = projMat * modelViewMat * terrainCoords;
		normal = normalize(normalMat * terrainNormals);
		lightDirection = normalize(vec3(light0.coords));
		colorsExport = globAmb*terrainFandB.ambRefl + max(dot(normal, lightDirection), 0.0f) * (light0.difCols * terrainFandB.difRefl);
		texCoordsExport = terrainTexCoords;
		//objectIDExport = switchOn;
	}
	else if (switchOn == 1)
	{
		gl_Position = projMat * modelViewMat * translationMat * treeCoords;
		colorsExport = treeColors;
		//objectIDExport = switchOn;
	}
}