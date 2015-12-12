#include "DiamondSquare.h"

DiamondSquare::DiamondSquare()
{
}

DiamondSquare::DiamondSquare(float **terrain, int mapSize, float seed, float height)
{
	std::cout << seed << std::endl;
	genRandSeed(seed);
	MAP_SIZE = mapSize;

	float e = terrain[0][0] = getRandNum(height);
	std::cout << "TEST: " << e << std::endl;
	terrain[MAP_SIZE-1][MAP_SIZE-1] = getRandNum(height);
	terrain[0][MAP_SIZE-1] = getRandNum(height);
	terrain[MAP_SIZE-1][0] = getRandNum(height);

	treeLocations = new List<glm::vec2>();

	std::cout << "CORNERS: 1: " << terrain[MAP_SIZE - 1][MAP_SIZE - 1] << " 2: " << terrain[0][MAP_SIZE - 1] << " 3: " << terrain[0][MAP_SIZE - 1] << " 4: " << terrain[MAP_SIZE - 1][0] << std::endl;
	std::cout << "CONSTRUCTED"<< std::endl;
	//printTerrain(terrain);
}


DiamondSquare::~DiamondSquare()
{
}

void::DiamondSquare::genTerrain(float **terrain, int x, int y, float range, int level)
{
	if (level < 1)
	{
		errorCheck(terrain);
		return;
	}
	diamond(terrain, x, y, range, level);
	square(terrain, x, y, range, level);
	//std::cout << "RANGE: " << range << " *100 " << int(range * 100) << " " << int(-range * 100) << " RAND EXAMPLE: " << rand()%int(range*100) + -range*100<< std::endl;
	genTerrain(terrain, x, y, range / 2.0f , level / 2);
}

void::DiamondSquare::diamond(float **terrain, int x, int y, float range, int level)
{
	for (int i = (x + level); i < x + MAP_SIZE; i += level)
	{
		for (int j = (y + level); j < y + MAP_SIZE; j += level)
		{
			float t1, t2, t3, t4;
			t1 = terrain[i - level][j - level];
			t2 = terrain[i][j - level];
			t3 = terrain[i - level][j];
			t4 = terrain[i][j];

			float midpoint = t1 + t2 + t3 + t4;
			midpoint /= 4;
			midpoint += getRandNum(range);

			terrain[i - level / 2][j - level / 2] = midpoint;
		}
	}
}

void::DiamondSquare::square(float **terrain, int x, int y, float range, int level)
{
	for (int i = (x + level); i < x + MAP_SIZE; i += level)
	{
		for (int j = (y + level); j < y + MAP_SIZE; j += level)
		{
			float t1, t2, t3, t4;
			t1 = terrain[i - level][j - level];
			t2 = terrain[i][j - level];
			t3 = terrain[i - level][j];
			t4 = terrain[i][j];

			float midpoint = terrain[i - level / 2][j - level / 2];
			float mid1, mid2, mid3, mid4;

			
			mid1 = t1 + t3 + midpoint;
			mid1 /= 3;
			mid1 += getRandNum(range);

			terrain[i - level][j - level / 2] = mid1;

			mid2 = t1 + t2 + midpoint;
			mid2 /= 3;
			mid2 += getRandNum(range);

			terrain[i - level / 2][j - level] = mid2;

			//
			//

			//mid3

			mid3 = t2 + t4 + midpoint;
			mid3 /= 3;
			mid3 += getRandNum(range);

			terrain[i - level / 2][j - level / 2] = mid3;


			//mid4
			mid4 = t3 + t4 + midpoint;
			mid4 /= 3;
			mid4 += getRandNum(range);

			terrain[i - level][j - level] = mid4;

		}
	}
}

float::DiamondSquare::getRandNum(float range)
{
	//float randNum = rand() %int(range*100) +-range*100;
	//float randNum = rand() % 100 + -100;
	//return (randNum / 100);
	//

	float randNum = ((float)rand() / (float)RAND_MAX);
	float newRange = range*2;

	float newHeight = (randNum*newRange) + -range;
	//newHeight = fmin(8, newHeight)
	
	return newHeight;
}

void::DiamondSquare::genRandSeed(float seed)
{
	if (seed == 0)
	{
		seed = rand()% 1000;
	}
	//seed = rand() % 1000;
	std::cout << seed << std::endl;
	srand(seed);
}

void::DiamondSquare::printTerrain(float **terrain)
{
	std::cout << "PRINTING ARRAY" << std::endl;
	std::cout << std::endl;
	for (int i = 0; i < MAP_SIZE; i++)
	{
		std::cout << "[";
		for (int j = 0; j < MAP_SIZE; j++)
		{
			std::cout << terrain[i][j] << ",";
		}
		std::cout << "]" << std::endl;
	}
	std::cout << std::endl;
}

float::DiamondSquare::getTerrainVal(float **terrain, int x, int z)
{
	if (x<MAP_SIZE && z<MAP_SIZE)
	{
		return terrain[x][z];
	}
	else
	{
		return terrain[MAP_SIZE][MAP_SIZE];
	}
	
	
}

void::DiamondSquare::errorCheck(float **terrain)
{
	int errorCount = 0;
	for (int i = 0; i < MAP_SIZE; i++)
	{
		for (int j = 0; j < MAP_SIZE; j++)
		{
			if (terrain[i][j] == 0)
			{
				std::cout << "ERROR AT: " << i << "," << j << " WITH VALUE: " << terrain[i][j] << std::endl;
				errorCount++;
			}
			if (terrain[i][j] > 0)
			{
				//std::cout << "POSITIVE VALUE AT: " << i << "," << j << " WITH VALUE: " << terrain[i][j] << std::endl;
			}
		}
	}
	std::cout << "TOTAL ERRORS: " << errorCount << std::endl;
}

void DiamondSquare::getRandomLocation(float **terrain)
{
	bool isGenerated = false;
	glm::vec2 *newLocation;
	while (!isGenerated)
	{
		cout << "GENERATING" << endl;
		float x = rand() % MAP_SIZE-1;
		float z = rand() % MAP_SIZE - 1;
		bool isExisting = false;
		for (int i = 0; i < treeLocations->getLength(); i++)
		{
			if (treeLocations->getNode(i)->value->x == x && treeLocations->getNode(i)->value->y == z)
			{
				isExisting = true;
			}
			

		}
		//environmental constrain // max height
		if (terrain[int(x)][int(z)] > -8)
		{
			isGenerated = false;
			cout << "STOPPED" << endl;
		}
		else if (!isExisting)
		{
			cout << "GENERATED: " << x << "," << z << endl;
			isGenerated = true;
			newLocation = new glm::vec2(x, z);
			Node<glm::vec2> *newTreeLocation = new Node<glm::vec2>(newLocation);
			treeLocations->insert(treeLocations->head, newTreeLocation);
		}
	}
}