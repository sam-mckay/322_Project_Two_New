#pragma once
#include<stdlib.h>
#include<iostream>
#include"glm.hpp"
#include"List.h"
class DiamondSquare
	/** Diamond Square
	*
	* Singluton class to randomly generate terrain
	*
	*/
{
public:
	DiamondSquare();
	DiamondSquare(float **terrain, int mapSize, float seed, float height);
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
	List<glm::vec2> *treeLocations;
	/** Generate Random seed*/
	void genRandSeed(float seed);
	/** Generate Random random number*/
	float getRandNum(float range);

	/**Print Terrain
	*
	* Prints all terrain values
	*
	*\param Terrain Pointer to Terrain the terrain 2D array
	*
	*/
	void printTerrain(float **terrain);

	/**Get Terrain Value
	*
	* Returns a specific terrain value if it exists
	*
	*\param Terrain Pointer to Terrain the terrain 2D array
	*\param x The value of the terrain
	*\param z The value of the terrain
	*
	*/
	float getTerrainVal(float **terrain, int x, int z);

	/**Get Random Location
	*
	* Returns a valid location within the terrain
	*
	*\param Terrain Pointer to Terrain the terrain 2D array
	*
	*/
	void getRandomLocation(float **terrain);

private:

	void squareSetPoint(float **terrain, int midPointX, int midPointY, int width);
	void errorCheck(float **terain);
};

