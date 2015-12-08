#pragma once
#include<stdlib.h>
#include<iostream>
class DiamondSquare
	/** Diamond Square
	*
	* Singluton class to randomly generate terrain
	*
	*/
{
public:
	DiamondSquare();
	DiamondSquare(float **terrain, int mapSize, float seed);
	~DiamondSquare();

	/** Generate Terrain
	*
	* Generates a new terrain
	*
	*/
	void genTerrain(float **terrain, int x, int y, float range, int level);


	/**Diamond
	*
	* Diamond section of diamond square algorithm
	*
	*\param Terrain Pointer to Terrain the terrain 2D array
	*\param x The X coord of the first point in the square
	*\param y The Y coord of the first point in the square
	*\param width The width of the square
	*
	*/
	void diamond(float **terrain, int x, int y, float range, int level);

	/**Sqaure
	*
	* Sqaure section of diamond square algorithm
	*
	*\param Terrain Pointer to Terrain the terrain 2D array
	*\param x The X coord of the first point in the square
	*\param y The Y coord of the first point in the square
	*\param width The width of the square
	*
	*/
	void square(float **terrain, int x, int y, float range, int level);


	int MAP_SIZE;
	/** Generate Random seed*/
	void genRandSeed(float seed);
	/** Generate Random random number*/
	float getRandNum(float range);

	void printTerrain(float **terrain);

private:

	void squareSetPoint(float **terrain, int midPointX, int midPointY, int width);
	void errorCheck(float **terain);
};

