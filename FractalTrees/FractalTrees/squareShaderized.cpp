////////////////////////////////////////////////////////          
// squareShaderized.cpp
//
// Forward-compatible core GL 4.3 version of square.cpp.
// 
// Sumanta Guha
////////////////////////////////////////////////////////

#include <iostream>
#include <fstream>

#include"glm.hpp"
#include"gtc/matrix_transform.hpp"
#include"gtc/type_ptr.hpp"
#include"glew.h"
#include<freeglut.h>
#include<glext.h>

#pragma comment(lib, "glew32.lib") 

using namespace std;
using namespace glm;

double ASPECT = 1;

static mat4 projMat = mat4(1.0);

vec3 eye, centre, up, los;
float cameraTheta = 180;
float cameraPhi = 0;

mat4 modelViewMat = mat4(1.0);


struct Vertex
{
   float coords[4];
   float colors[4];
};

struct Matrix4x4
{
   float entries[16];
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

static enum buffer {SQUARE_VERTICES}; 
static enum object {SQUARE}; 

int indexBuffers[30] = {0,1,1, 2, 1, 3, 2, 4, 2, 5, 3, 6, 3, 7, 4, 8, 4, 9, 5, 10, 5, 11, 6, 12, 6, 13, 7, 14, 7, 15 };
int indexData[7] = { 2, 6, 10, 14, 18, 22, 26 };
int levelBuffer[7] = { 1, 2, 2, 3, 3, 3, 3 };
//int indexBuffers[28] = {1, 2, 1, 3, 2, 4, 2, 5, 3, 6, 3, 7, 4, 8, 4, 9, 5, 10, 5, 11, 6, 12, 6, 13, 7, 14, 7, 15 };

//int indexBuffers[16] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 };

const int MAX_LEVEL = 3;

// Globals
Vertex squareVertices[16];

static unsigned int
   programId,
   vertexShaderId,
   fragmentShaderId,
   modelViewMatLoc,
   projMatLoc,
   buffer[1],
   vao[1];
   
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

float getRandAngle(float range)
{
	float randNum = ((float)rand() / (float)RAND_MAX);
	float newRange = range * 2;
	return (randNum*newRange) + -range;
}

void drawTrunk(Vertex treeTrunk1)
{
	squareVertices[0].coords[0] = treeTrunk1.coords[0];
	squareVertices[0].coords[1] = treeTrunk1.coords[1];
	squareVertices[0].coords[2] = 0.0;
	squareVertices[0].coords[3] = 1.0;
	
	squareVertices[1].coords[0] = treeTrunk1.coords[2];
	squareVertices[1].coords[1] = treeTrunk1.coords[3];
	squareVertices[1].coords[2] = 0.0;
	squareVertices[1].coords[3] = 1.0;

	squareVertices[0].colors[0] = treeTrunk1.colors[0];
	squareVertices[0].colors[1] = treeTrunk1.colors[1];
	squareVertices[0].colors[2] = treeTrunk1.colors[2];
	squareVertices[0].colors[3] = treeTrunk1.colors[3];

	squareVertices[1].colors[0] = treeTrunk1.colors[0];
	squareVertices[1].colors[1] = treeTrunk1.colors[1];
	squareVertices[1].colors[2] = treeTrunk1.colors[2];
	squareVertices[1].colors[3] = treeTrunk1.colors[3];

}

int calcBranch(Vertex Branch, int previousIndex, float angle, bool right, float height)
{
	int index = previousIndex * 2;
	
	Branch.coords[0] = squareVertices[previousIndex].coords[0] - squareVertices[0].coords[0];
	Branch.coords[1] = squareVertices[previousIndex].coords[1] - squareVertices[0].coords[1];

	Branch.coords[0] *= height;
	Branch.coords[1] *= height;

	float newAngle = convertToRad(angle);
	mat2x2 rotatationMatrix;

	int r = sqrt(pow(Branch.coords[0], 2) + pow(Branch.coords[1], 2) + pow(Branch.coords[2], 2));
	if (right)
	{
		index += 1;
		newAngle = -newAngle / 2.0f;
		rotatationMatrix = mat2x2{ cos(newAngle), sin(newAngle), -sin(newAngle), cos(newAngle) };
	}
	else
	{
		newAngle = newAngle / 2.0f;
		rotatationMatrix = mat2x2{ cos(newAngle), sin(newAngle), -sin(newAngle), cos(newAngle) };
	}
	
	glm::vec2 source(Branch.coords[0], Branch.coords[1]);
	
	source = rotatationMatrix*source;

	Branch.coords[2] = squareVertices[previousIndex].coords[0] + source.x;
	Branch.coords[3] = squareVertices[previousIndex].coords[1] + source.y;

	glm::vec2 end(Branch.coords[2], Branch.coords[3]);

	squareVertices[index].coords[0] = end.x;
	squareVertices[index].coords[1] = end.y;
	squareVertices[index].coords[2] = 0.0;
	squareVertices[index].coords[3] = 1.0;
	
	squareVertices[previousIndex].colors[0] = Branch.colors[0];
	squareVertices[previousIndex].colors[1] = Branch.colors[1];
	squareVertices[previousIndex].colors[2] = Branch.colors[2];
	squareVertices[previousIndex].colors[3] = Branch.colors[3];

	squareVertices[index].colors[0] = Branch.colors[0];
	squareVertices[index].colors[1] = Branch.colors[1];
	squareVertices[index].colors[2] = Branch.colors[2];
	squareVertices[index].colors[3] = Branch.colors[3];

	if (right)
	{
		cout << "DRAWN RIGHT BRANCH : PREVIOUS INDEX: " << previousIndex << " INDEX: " << index << endl;
	}
	else
	{
		cout << "DRAWN LEFT BRANCH  : PREVIOUS INDEX: " << previousIndex << " INDEX: " << index << endl;
	}
	return index;
}

void drawBranch(int level, float prevPosX, float prevPosY, float height, float angle, int prevIndex)
{
	Vertex Branch = Vertex{ { 0, 0, 0, 0 }, { 0.55f, 0.27f, 0.075f, 1.0f } };
	cout << "LEVEL: " << level << " MAX LEVEL: " << MAX_LEVEL << endl;

	int leftIndex = calcBranch(Branch, prevIndex, angle+=getRandAngle(15), false, height);

	if (level < MAX_LEVEL-1)
	{
		//angle += getRandAngle(15);
		prevPosX = squareVertices[leftIndex].coords[0];
		prevPosY = squareVertices[leftIndex].coords[1];

		float prevPosX1, prevPosX2, prevPosY1, prevPosY2;

		prevPosX1 = squareVertices[prevIndex].coords[0];
		prevPosX2 = squareVertices[leftIndex].coords[0];

		prevPosX = prevPosX2 - prevPosX1;
		
		drawBranch(level + 1, prevPosX, prevPosY + (height / 2.0), height / 2.0, angle/2.0, leftIndex);
	}
	
	int rightIndex = calcBranch(Branch, prevIndex, angle += getRandAngle(15), true, height);
	if (level < MAX_LEVEL-1)
	{
		//angle += getRandAngle(15);
		prevPosX = squareVertices[rightIndex].coords[0];
		prevPosY = squareVertices[rightIndex].coords[1];

		float prevPosX1, prevPosX2, prevPosY1, prevPosY2;

		prevPosX1 = squareVertices[prevIndex].coords[0];
		prevPosX2 = squareVertices[rightIndex].coords[0];

		prevPosX = prevPosX2 - prevPosX1;

		drawBranch(level + 1, prevPosX, prevPosY + (height / 2.0), height / 2.0, angle / 2.0, rightIndex);
	}
	
}

//generates a tree
void drawTree(int *level)
{	
	Vertex Trunk = Vertex{ { 0, -30, 0, -15 }, {0.55f, 0.27f, 0.075f, 1.0f} };
	drawTrunk(Trunk);


	//drawBranch(0, -0, -15, 15, 40, 0);
	float height = 1.0;
	float angle = 90;
	drawBranch(0, 0, -15 + height, height, angle, 1);

	//test
	for (int i = 0; i < 15; i++)
	{
		cout << squareVertices[i].coords[0] << "," << squareVertices[i].coords[1] << endl;
	}
	
	cout << "DONE TREE" << endl;
}

// Initialization routine.
void setup(void)
{
   glClearColor(1.0, 1.0, 1.0, 0.0);
   //
   //
   int levelVal = 0;
   int *level = &levelVal;
   
   drawTree(level);
   //

   // Create shader program executable.
   char* vertexShader = readTextFile("vertexShader.glsl");
   vertexShaderId = glCreateShader(GL_VERTEX_SHADER); 
   glShaderSource(vertexShaderId, 1, (const char**) &vertexShader, NULL); 
   glCompileShader(vertexShaderId); 

   char* fragmentShader = readTextFile("fragmentShader.glsl");
   fragmentShaderId = glCreateShader(GL_FRAGMENT_SHADER);
   glShaderSource(fragmentShaderId, 1, (const char**) &fragmentShader, NULL); 
   glCompileShader(fragmentShaderId); 

   programId = glCreateProgram(); 
   glAttachShader(programId, vertexShaderId); 
   glAttachShader(programId, fragmentShaderId); 
   glLinkProgram(programId); 
   glUseProgram(programId); 
   ///////////////////////////////////////

   // Create VAO and VBO and associate data with vertex shader.
   glGenVertexArrays(1, vao);
   glGenBuffers(1, buffer);
   glBindVertexArray(vao[SQUARE]);
   glBindBuffer(GL_ARRAY_BUFFER, buffer[SQUARE_VERTICES]);
   glBufferData(GL_ARRAY_BUFFER, sizeof(squareVertices), squareVertices, GL_STATIC_DRAW);

   glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(squareVertices[0]), 0);
   glEnableVertexAttribArray(0);
   glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(squareVertices[0]), (GLvoid*)sizeof(squareVertices[0].coords));
   glEnableVertexAttribArray(1);
   ///////////////////////////////////////

	//
   projMatLoc = glGetUniformLocation(programId, "projMat");
   //projMat = ortho(-50.0, 50.0, -50.0, 50.0, -1.0, 1.0);
   projMat = perspective(1.0472, (double)ASPECT, 0.1, 200.0);
   glUniformMatrix4fv(projMatLoc, 1, GL_FALSE, value_ptr(projMat));
   // Obtain modelview matrix uniform location and set value.
   //Matrix4x4 modelViewMat = IDENTITY_MATRIX4x4;
   //modelViewMatLoc = glGetUniformLocation(programId, "modelViewMat"); 
   //glUniformMatrix4fv(modelViewMatLoc, 1, GL_TRUE, modelViewMat.entries);

   //mat4 modelViewMat = mat4(1.0);
   eye = vec3(0.0, 0.0, -65.0);
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
   glClear(GL_COLOR_BUFFER_BIT);
   glLineWidth(4);
  
   


   modelViewMat = lookAt(eye, los + eye, up);
   modelViewMatLoc = glGetUniformLocation(programId, "modelViewMat");
   glUniformMatrix4fv(modelViewMatLoc, 1, GL_FALSE, value_ptr(modelViewMat));

   //cout << "DRAWING" << endl;
   glDrawElements(GL_LINES, 30, GL_UNSIGNED_INT, &indexBuffers);
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
