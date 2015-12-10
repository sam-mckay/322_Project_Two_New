////////////////////////////////////////////////////////          
// squareShaderized.cpp
//
// Forward-compatible core GL 4.3 version of square.cpp.
// 
// Sumanta Guha
////////////////////////////////////////////////////////

#include <iostream>
#include <fstream>
#include <vector>

#include"glm.hpp"
#include"gtc/matrix_transform.hpp"
#include"gtc/type_ptr.hpp"
#include"glew.h"
#include<freeglut.h>
#include<glext.h>

#include"Tree.h"
#include"DiamondSquare.h"
#include"getbmp.h"

#pragma comment(lib, "glew32.lib") 

using namespace std;
using namespace glm;



double ASPECT = 1;

static mat4 projMat = mat4(1.0);

vec3 eye, centre, up, los;
float cameraTheta = 180;
float cameraPhi = 0;

mat4 modelViewMat = mat4(1.0);
mat4 translationMat = mat4x4(1.0);

const int MAP_SIZE = 33;
float height = 10;
float NUM_TREES = 20;
Tree *tree1;

DiamondSquare *terrainGen;

struct Vertex
{
	int objectID;
	float coords[4];
	float normals[3];
	float texcoords[2];
	float colors[4];
};

struct Matrix4x4
{
   float entries[16];
};


struct Material
{
	vec4 ambRefl;
	vec4 difRefl;
	vec4 specRefl;
	vec4 emitCols;
	float shininess;
};

struct Light
{
	vec4 ambCols;
	vec4 difCols;
	vec4 specCols;
	vec4 coords;
};

static const Material terrainFandB =
{
	vec4(2.0, 2.0, 2.0, 1.0),
	vec4(1.0, 1.0, 1.0, 1.0),
	vec4(1.0, 1.0, 1.0, 1.0),
	vec4(0.0, 0.0, 0.0, 1.0),
	50.0f
};


static const Matrix4x4 IDENTITY_MATRIX4x4 = 
{
   {
      1.0, 0.0, 0.0, 0.0,
      0.0, 1.0, 0.0, 0.0,
      0.0, 0.0, 1.0, 0.0,
      0.0, 0.0, 0.0, 1.0
   }
};

//lighting

static const vec4 globAmb = glm::vec4(0.2, 0.2, 0.2, 1.0);
static const Light light0 =
{
	vec4(0.5, 0.5, 0.5, 1.0),
	vec4(1.0, 1.0, 1.0, 1.0),
	vec4(1.0, 1.0, 1.0, 1.0),
	vec4(1.0, 5.0, 0.0, 0.0)
};

//texturing
static BitMapFile *grassTerrain[1];


//buffers
static enum buffer { TERRAIN_VERTICES, TREE_VERTICES };
static enum object { TERRAIN, TREE };



static Vertex terrainVertices[MAP_SIZE*MAP_SIZE] = {};
static glm::vec3 terrainTriangleNormals[((MAP_SIZE-1)*(MAP_SIZE-1))*2] = {};
static mat3 normalMat = mat3(1.0);
//static Vertex terrainVerticeNormals[MAP_SIZE*MAP_SIZE] = {};

const int numStripsRequired = MAP_SIZE - 1;
const int verticesPerStrip = 2 * MAP_SIZE;

unsigned int terrainIndexData[numStripsRequired][verticesPerStrip];

int indexBuffers[30] = {0,1,1, 2, 1, 3, 2, 4, 2, 5, 3, 6, 3, 7, 4, 8, 4, 9, 5, 10, 5, 11, 6, 12, 6, 13, 7, 14, 7, 15 };
int indexData[7] = { 2, 6, 10, 14, 18, 22, 26 };
int levelBuffer[7] = { 1, 2, 2, 3, 3, 3, 3 };


const int MAX_LEVEL = 3;

// Globals
Vertex squareVertices[16];
float **terrain = new float*[MAP_SIZE];


static unsigned int
   programId,
   vertexShaderId,
   fragmentShaderId,
   modelViewMatLoc,
   projMatLoc,
   normalMatLoc,
   translationMatLoc,
   buffer[2],
   vao[2],
   texture[1],
   grassTexLoc;
   
// Function to read text file.
char* readTextFile(char* aTextFile)
{
   FILE* filePointer = fopen(aTextFile, "rb");	
   char* content = NULL;
   long numVal = 0;

   fseek(filePointer, 0L, SEEK_END);
   numVal = ftell(filePointer);
   fseek(filePointer, 0L, SEEK_SET);
   content = (char*) malloc((numVal+1) * sizeof(char)); 
   fread(content, 1, numVal, filePointer);
   content[numVal] = '\0';
   fclose(filePointer);
   return content;
}

void shaderCompileTest(GLuint shader)
{
	GLint result = GL_FALSE;
	int logLength;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &result);
	glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLength);
	std::vector<GLchar> vertShaderError((logLength > 1) ? logLength : 1);
	glGetShaderInfoLog(shader, logLength, NULL, &vertShaderError[0]);
	std::cout << &vertShaderError[0] << std::endl;
}

float convertToRad(float angle)
{
	return angle*3.14/180;
}

//
//
//
//
//
//
//

vec3 moveTree(int x, int z)
{
	return vec3(x, terrain[x][z], z);
}

void getTriangleNormal()
{

	for (int i = 0; i < ((MAP_SIZE - 1)*(MAP_SIZE - 1)) * 2; i)
	{
		glm::vec3 point1, point2, point3;

		point1 = glm::vec3(terrainVertices[i].coords[0], terrainVertices[i].coords[1], terrainVertices[i].coords[2]);
		point2 = glm::vec3(terrainVertices[i + 1].coords[0], terrainVertices[i + 1].coords[1], terrainVertices[i + 1].coords[2]);
		point3 = glm::vec3(terrainVertices[i + 2].coords[0], terrainVertices[i + 2].coords[1], terrainVertices[i + 2].coords[2]);
		
		//cout << "TERRAIN INTEGRITY CHECK: " << terrainVertices[i].coords[0] << "," << terrainVertices[i].coords[1] << "," << terrainVertices[i].coords[2] << endl;

		//cout << "point1 Vector: " << point1.x << "," << point1.y << "," << point1.z << endl;
		//cout << "point2 Vector: " << point2.x << "," << point2.y << "," << point2.z << endl;
		//cout << "point3 Vector: " << point3.x << "," << point3.y << "," << point3.z << endl;

		//side1 = b - a
		glm::vec3 aToB = point2 - point1;
		//side2 = c - a
		glm::vec3 aToC = point3 - point1;

		//cout << "AtoB Vector: " << aToB.x << "," << aToB.y << "," << aToB.z << endl;
		//cout << "AtoC Vector: " << aToC.x << "," << aToC.y << "," << aToC.z << endl;

		terrainTriangleNormals[i] = glm::cross(aToB, aToC);
		//cout << "CROSS PRODUCT OUTPUT " << glm::cross(aToB, aToC).x << "," << glm::cross(aToB, aToC).y << "," << glm::cross(aToB, aToC).z << endl;
		i += 2;
		//cout << i << endl;
	}

	for (int i = 0; i < ((MAP_SIZE - 1)*(MAP_SIZE - 1)) * 2; i++)
	{		
		cout << "TRIANGLE NORMALS: " << terrainTriangleNormals[i].x << "," << terrainTriangleNormals[i].y << "," << terrainTriangleNormals[i].z << endl;
		cout << i << endl;
	}
	
}

glm::vec3 getVertexNormal(int x, float y, int z, int i)
{
		glm::vec3 point1, point2, point3;

		point1 = glm::vec3(x,y,z);

		if (x != MAP_SIZE-1 && z != MAP_SIZE-1)
		{
			point3 = glm::vec3(float(x + 1), terrain[int(x + 1)][int(z)], float(z));
			point2 = glm::vec3(float(x), terrain[int(x)][int(z + 1)], float(z + 1));
		}
		else if (x != MAP_SIZE - 1 && z == MAP_SIZE - 1)
		{
			point2 = glm::vec3(float(x + 1), terrain[int(x + 1)][int(z)], float(z));
			point3 = glm::vec3(float(x), terrain[int(x)][int(z - 1)], float(z - 1));
		}
		else if (x == MAP_SIZE - 1 && z != MAP_SIZE - 1)
		{
			point2 = glm::vec3(float(x - 1), terrain[int(x - 1)][int(z)], float(z));
			point3 = glm::vec3(float(x), terrain[int(x)][int(z + 1)], float(z + 1));
		}
		else 
		{
			point3 = glm::vec3(float(x - 1), terrain[int(x - 1)][int(z)], float(z));
			point2 = glm::vec3(float(x), terrain[int(x)][int(z - 1)], float(z - 1));
		}

		//side1 = b - a
		glm::vec3 aToB = point2 - point1;
		//side2 = c - a
		glm::vec3 aToC = point3 - point1;

		glm::vec3 result = glm::cross(aToB, aToC);

		return glm::normalize(result);	
}

void lightSetup()
{
	//getTriangleNormal();
	//global
	
	glUniform4fv(glGetUniformLocation(programId, "globAmb"),1, &globAmb[0]);

	//lights
	glUniform4fv(glGetUniformLocation(programId, "light0.ambCols"), 1,
		&light0.ambCols[0]);
	glUniform4fv(glGetUniformLocation(programId, "light0.difCols"), 1,
		&light0.difCols[0]);
	glUniform4fv(glGetUniformLocation(programId, "light0.specCols"), 1,
		&light0.specCols[0]);
	glUniform4fv(glGetUniformLocation(programId, "light0.coords"), 1,
		&light0.coords[0]);
		

	//terrain
	glUniform4fv(glGetUniformLocation(programId, "terrainFandB.ambRefl"), 1,
		&terrainFandB.ambRefl[0]);
	glUniform4fv(glGetUniformLocation(programId, "terrainFandB.difRefl"), 1,
		&terrainFandB.difRefl[0]);
	glUniform4fv(glGetUniformLocation(programId, "terrainFandB.specRefl"), 1,
		&terrainFandB.specRefl[0]);
	glUniform4fv(glGetUniformLocation(programId, "terrainFandB.emitCols"), 1,
		&terrainFandB.emitCols[0]);
	glUniform1f(glGetUniformLocation(programId, "terrainFandB.shininess"),
		terrainFandB.shininess);


	
}

void loadImages()
{
	//load images
	grassTerrain[0] = getbmp("Assets/highlands.bmp");

	//create texture IDs
	glGenTextures(1, texture);

	//bind grass image
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture[0]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, grassTerrain[0]->sizeX, grassTerrain[0]->sizeY, 0,
		GL_RGBA, GL_UNSIGNED_BYTE, grassTerrain[0]->data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glGenerateMipmap(GL_TEXTURE_2D);
	grassTexLoc = glGetUniformLocation(programId, "grassTex");
	glUniform1i(grassTexLoc, 0);
}

void terrainSetup()
{
	//create terrain
	//create terrain array pointer

	for (int i = 0; i < MAP_SIZE; i++)
	{
		terrain[i] = new float[MAP_SIZE];
	}

	// Initialise terrain - set values in the height map to 0
	for (int x = 0; x < MAP_SIZE; x++)
	{
		for (int z = 0; z < MAP_SIZE; z++)
		{
			terrain[x][z] = 0;
		}
	}

	terrainGen = new DiamondSquare(terrain, MAP_SIZE, 4, height);
	terrainGen->genTerrain(terrain, 0, 0, height, (MAP_SIZE - 1));//7
	//terrainGen->printTerrain(terrain);

	// Intialise vertex array
	int i = 0;
	// texture init
	//generate texture co-ordinates
	float fTextureS = float(MAP_SIZE)*0.1f;
	float fTextureT = float(MAP_SIZE)*0.1f;

	for (int z = 0; z < MAP_SIZE; z++)
	{
		for (int x = 0; x < MAP_SIZE; x++)
		{
			// Set the coords (1st 4 elements) and a default colour of black (2nd 4 elements)
			glm::vec3 result = getVertexNormal(x, terrain[x][z], z, i);
			terrainVertices[i] = { 0, { (float)x, terrain[x][z], (float)z, 1.0 }, { result.x, result.y, result.z }, {} };

			// cout << "TERRAIN INITIALISED STATUS: " << terrainVertices[i].coords[0] << "," << terrainVertices[i].coords[1] << "," << terrainVertices[i].coords[2] << endl;

			float fScaleC = float(x) / float(MAP_SIZE - 1);
			float fScaleR = float(z) / float(MAP_SIZE - 1);

			terrainVertices[i].texcoords[0] = (fTextureS*fScaleC);
			terrainVertices[i].texcoords[1] = (fTextureT*fScaleR);

			i++;

		}
	}

	// Now build the index data 
	i = 0;
	for (int z = 0; z < MAP_SIZE - 1; z++)
	{
		i = z * MAP_SIZE;
		for (int x = 0; x < MAP_SIZE * 2; x += 2)
		{
			terrainIndexData[z][x] = i;
			i++;
		}
		for (int x = 1; x < MAP_SIZE * 2 + 1; x += 2)
		{
			terrainIndexData[z][x] = i;
			i++;
		}
	}
}

// Initialization routine.
void setup(void)
{
   glClearColor(1.0, 1.0, 1.0, 0.0);
   //
   //
   loadImages();

   //create Trees
   int levelVal = 0;
   int *level = &levelVal;
   
   tree1 = new Tree(MAX_LEVEL);
   terrainSetup();
   for (int i = 0; i < NUM_TREES; i++)
   {
	   terrainGen->getRandomLocation(terrain);
   }

   tree1->drawTree();
   

   //depth test

   glEnable(GL_DEPTH_TEST);
   glEnable(GL_CULL_FACE);
   glCullFace(GL_BACK);

   //
   


   // Create shader program executable.
   cout << "VERTEX" << endl;
   char* vertexShader = readTextFile("vertexShader.glsl");
   vertexShaderId = glCreateShader(GL_VERTEX_SHADER); 
   glShaderSource(vertexShaderId, 1, (const char**) &vertexShader, NULL); 
   glCompileShader(vertexShaderId); 
   
   shaderCompileTest(vertexShaderId);
   cout << "FRAGMENT" << endl;
   char* fragmentShader = readTextFile("fragmentShader.glsl");
   fragmentShaderId = glCreateShader(GL_FRAGMENT_SHADER);
   glShaderSource(fragmentShaderId, 1, (const char**) &fragmentShader, NULL); 
   glCompileShader(fragmentShaderId); 

   shaderCompileTest(fragmentShaderId);

   programId = glCreateProgram(); 
   glAttachShader(programId, vertexShaderId); 
   glAttachShader(programId, fragmentShaderId); 
   glLinkProgram(programId); 
   glUseProgram(programId); 
   ///////////////////////////////////////


   // Create VAO and VBO and associate data with vertex shader.
   ///////////////////////////////////////
   glGenVertexArrays(2, vao);
   glGenBuffers(2, buffer);

   //TERRAIN
   // Create vertex array object (VAO) and vertex buffer object (VBO) and associate data with vertex shader.
   glBindVertexArray(vao[TERRAIN]);
   glBindBuffer(GL_ARRAY_BUFFER, buffer[TERRAIN_VERTICES]);
   glBufferData(GL_ARRAY_BUFFER, sizeof(terrainVertices), terrainVertices, GL_STATIC_DRAW);

   glVertexAttribPointer(0, 1, GL_FLOAT, GL_FALSE, sizeof(terrainVertices[0]), 0);
   glEnableVertexAttribArray(0);
   glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(terrainVertices[0]), (GLvoid*)sizeof(terrainVertices[0].objectID));
   glEnableVertexAttribArray(1);
   glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(terrainVertices[0]), (GLvoid*)(sizeof(terrainVertices[0].objectID) + sizeof(terrainVertices[0].coords)));
   glEnableVertexAttribArray(2);
   glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(terrainVertices[0]), (GLvoid*)(sizeof(terrainVertices[0].objectID) + sizeof(terrainVertices[0].coords) + sizeof(terrainVertices[0].normals)));
   glEnableVertexAttribArray(3);
   glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(terrainVertices[0]), (GLvoid*)(sizeof(terrainVertices[0].objectID) + sizeof(terrainVertices[0].coords) + sizeof(terrainVertices[0].normals)
	   +sizeof(terrainVertices[0].texcoords)));
   glEnableVertexAttribArray(4);

   
   //TREE

   
   glBindVertexArray(vao[TREE]);
   glBindBuffer(GL_ARRAY_BUFFER, buffer[TREE_VERTICES]);
   glBufferData(GL_ARRAY_BUFFER, sizeof(tree1->squareVertices), tree1->squareVertices, GL_STATIC_DRAW);

   glVertexAttribPointer(5, 1, GL_FLOAT, GL_FALSE, sizeof(tree1->squareVertices[0]), 0);
   glEnableVertexAttribArray(5);
   glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(tree1->squareVertices[0]), (GLvoid*)(sizeof(tree1->squareVertices[0].objectID)));
   glEnableVertexAttribArray(6);
   glVertexAttribPointer(7, 3, GL_FLOAT, GL_FALSE, sizeof(tree1->squareVertices[0]), (GLvoid*)(sizeof(tree1->squareVertices[0].objectID) + sizeof(tree1->squareVertices[0].coords)));
   glEnableVertexAttribArray(7);
   glVertexAttribPointer(8, 2, GL_FLOAT, GL_FALSE, sizeof(tree1->squareVertices[0]), (GLvoid*)(sizeof(tree1->squareVertices[0].objectID) + sizeof(tree1->squareVertices[0].coords)
	   + sizeof(tree1->squareVertices[0].normals)));
   glEnableVertexAttribArray(8);
   glVertexAttribPointer(9, 4, GL_FLOAT, GL_FALSE, sizeof(tree1->squareVertices[0]), (GLvoid*)(sizeof(tree1->squareVertices[0].objectID) + sizeof(tree1->squareVertices[0].coords)
	   + sizeof(tree1->squareVertices[0].normals) + sizeof(tree1->squareVertices[0].texcoords)));
   glEnableVertexAttribArray(9);
   

   //TREE LIST 
   
   ///////////////////////////////////////

   lightSetup();
	//
   projMatLoc = glGetUniformLocation(programId, "projMat");
   normalMatLoc = glGetUniformLocation(programId, "normalMat");
   //projMat = ortho(-50.0, 50.0, -50.0, 50.0, -1.0, 1.0);
   projMat = perspective(1.0472, (double)ASPECT, 0.1, 200.0);
   glUniformMatrix4fv(projMatLoc, 1, GL_FALSE, value_ptr(projMat));
   // Obtain modelview matrix uniform location and set value.
   //Matrix4x4 modelViewMat = IDENTITY_MATRIX4x4;
   //modelViewMatLoc = glGetUniformLocation(programId, "modelViewMat"); 
   //glUniformMatrix4fv(modelViewMatLoc, 1, GL_TRUE, modelViewMat.entries);

   //mat4 modelViewMat = mat4(1.0);
   eye = vec3(15.3, 3.2, -1.3);
   //centre = vec3(0.0, 0.0, -64.0);
   los = vec3(0.0, 0.0, 1.0);
   up = vec3(0.0, 1.0, 0.0);
   modelViewMat = lookAt(eye, los+eye, up);
   modelViewMatLoc = glGetUniformLocation(programId, "modelViewMat");
   glUniformMatrix4fv(modelViewMatLoc, 1, GL_FALSE, value_ptr(modelViewMat));
   ///////////////////////////////////////

}


// Drawing routine.
void drawScene(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLineWidth(4);

	modelViewMat = lookAt(eye, los + eye, up);
	modelViewMatLoc = glGetUniformLocation(programId, "modelViewMat");
	glUniformMatrix4fv(modelViewMatLoc, 1, GL_FALSE, value_ptr(modelViewMat));
	normalMat = transpose(inverse(mat3(modelViewMat)));
	glUniformMatrix3fv(normalMatLoc, 1, GL_FALSE, value_ptr(normalMat));

   //cout << "DRAWING" << endl;
	//draw tree
	glUniform1i(glGetUniformLocation(programId, "switchOn"), 1);
	float x, z;
	//draw all trees in list
	for (int i = 0; i < NUM_TREES; i++)
	{
		x = terrainGen->treeLocations->getNode(i)->value->x;
		z = terrainGen->treeLocations->getNode(i)->value->y;
		float y = terrainGen->getTerrainVal(terrain, int(x), int(z));
		
		
		translationMat = translate(mat4(1), vec3(x, y, z));
		translationMatLoc = glGetUniformLocation(programId, "translationMat");
		glUniformMatrix4fv(translationMatLoc, 1, GL_FALSE, value_ptr(translationMat));

		glBindVertexArray(vao[TREE]);
		glBindBuffer(GL_ARRAY_BUFFER, buffer[TREE_VERTICES]);
		glDrawElements(GL_LINES, 30, GL_UNSIGNED_INT, indexBuffers);
	}


	//draw terrain
	// For each row - draw the triangle strip

	glUniform1i(glGetUniformLocation(programId, "switchOn"), 0);
	glBindVertexArray(vao[TERRAIN]);
	glBindBuffer(GL_ARRAY_BUFFER, buffer[TERRAIN_VERTICES]);
	for (int i = 0; i < MAP_SIZE - 1; i++)
	{
		glDrawElements(GL_TRIANGLE_STRIP, verticesPerStrip, GL_UNSIGNED_INT, terrainIndexData[i]);
	}


   glFlush();
   glutPostRedisplay();
}

// OpenGL window reshape routine.
void resize(int w, int h)
{
   glViewport(0, 0, w, h);
}

// Keyboard input processing routine.
void keyInput(unsigned char key, int x, int y)
{
   switch(key) 
   {
      case 27:
         exit(0);
         break;
	  case 119://w
		  eye.z += los.z *0.30;
		  eye.x += los.x *0.30;
		  eye.y += los.y *0.30;
		  break;
	  case 115://s
		  eye.z -= los.z *0.30;
		  eye.x -= los.x *0.30;
		  eye.y -= los.y *0.30;
		  break;
	  case 97://a
		  cameraTheta -= 1;
		  break;
	  case 100://d
		  cameraTheta += 1;
		  break;
	  case 113://q
		  cameraPhi += 1;
		  break;
	  case 101://e
		  cameraPhi -= 1;
		  break;
      default:
         break;
   }
   los.x = cos(radians(cameraPhi)) * sin(radians(cameraTheta));
   los.y = sin(radians(cameraPhi));
   los.z = cos(radians(cameraPhi)) * -cos(radians(cameraTheta));

   cout << "EYE :" << eye.x << "," << eye.y << "," << eye.z << endl;
}

// Main routine.
int main(int argc, char* argv[])
{	
   glutInit(&argc, argv);

   glutInitContextVersion(4, 2);
   glutInitContextProfile(GLUT_CORE_PROFILE);
   //glutInitContextFlags(GLUT_FORWARD_COMPATIBLE);
   
   glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);
   glutInitWindowSize(500, 500);
   glutInitWindowPosition(100, 100); 
   glutCreateWindow("squareShaderized.cpp");

   glutDisplayFunc(drawScene);
   glutReshapeFunc(resize);
   glutKeyboardFunc(keyInput);
   
   glewExperimental = GL_TRUE;
   glewInit();

   setup();

   glutMainLoop();
}
