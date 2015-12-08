#include"glm.hpp"
#include<iostream>
#pragma once

class Tree
{
public:
	Tree();
	Tree(int MAX_LEVEL);
	~Tree();
	struct Vertex{
		float coords[4];
		float colors[4];
	};

	int MAX_LEVEL;
	Vertex squareVertices[16];

	void drawTrunk(Vertex treeTrunk1);
	int calcBranch(Vertex Branch, int previousIndex, float angle, bool right, float height);
	void drawBranch(int level, float prevPosX, float prevPosY, float height, float angle, int prevIndex);
	void drawTree();
};

