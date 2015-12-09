#include "Tree.h"
using namespace std;

Tree::Tree()
{
}

Tree::Tree(int max_lvl)
{
	MAX_LEVEL = max_lvl;
}

Tree::~Tree()
{
}

struct Vertex
{
	int objectID;
	float coords[4];
	float normals[3];
	float texcoords[2];
	float colors[4];
};


float getRandAngle(float range)
{
	float randNum = ((float)rand() / (float)RAND_MAX);
	float newRange = range * 2;
	return (randNum*newRange) + -range;
}

void::Tree::drawTrunk(Vertex treeTrunk1)
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

int::Tree::calcBranch(Vertex Branch, int previousIndex, float angle, bool right, float height)
{
	int index = previousIndex * 2;

	Branch.coords[0] = squareVertices[previousIndex].coords[0] - squareVertices[0].coords[0];
	Branch.coords[1] = squareVertices[previousIndex].coords[1] - squareVertices[0].coords[1];

	Branch.coords[0] *= height;
	Branch.coords[1] *= height;

	float newAngle = glm::radians(angle);
	glm::mat2x2 rotatationMatrix;

	int r = sqrt(pow(Branch.coords[0], 2) + pow(Branch.coords[1], 2) + pow(Branch.coords[2], 2));
	if (right)
	{
		index += 1;
		newAngle = -newAngle / 2.0f;
		rotatationMatrix = glm::mat2x2{ cos(newAngle), sin(newAngle), -sin(newAngle), cos(newAngle) };
	}
	else
	{
		newAngle = newAngle / 2.0f;
		rotatationMatrix = glm::mat2x2{ cos(newAngle), sin(newAngle), -sin(newAngle), cos(newAngle) };
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

void::Tree::drawBranch(int level, float prevPosX, float prevPosY, float height, float angle, int prevIndex)
{
	Vertex Branch = Vertex{ 1, { 0, 0, 0, 0 }, {}, {}, { 0.55f, 0.27f, 0.075f, 1.0f } };
	cout << "LEVEL: " << level << " MAX LEVEL: " << MAX_LEVEL << endl;

	int leftIndex = calcBranch(Branch, prevIndex, angle += getRandAngle(15), false, height);

	if (level < MAX_LEVEL - 1)
	{
		//angle += getRandAngle(15);
		prevPosX = squareVertices[leftIndex].coords[0];
		prevPosY = squareVertices[leftIndex].coords[1];

		float prevPosX1, prevPosX2, prevPosY1, prevPosY2;

		prevPosX1 = squareVertices[prevIndex].coords[0];
		prevPosX2 = squareVertices[leftIndex].coords[0];

		prevPosX = prevPosX2 - prevPosX1;

		drawBranch(level + 1, prevPosX, prevPosY + (height / 2.0), height / 2.0, angle / 2.0, leftIndex);
	}

	int rightIndex = calcBranch(Branch, prevIndex, angle += getRandAngle(15), true, height);
	if (level < MAX_LEVEL - 1)
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


void::Tree::drawTree()
{
	Vertex Trunk = Vertex{ 1, { 0, -30, 0, -15 }, {}, {}, { 0.55f, 0.27f, 0.075f, 1.0f } };
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