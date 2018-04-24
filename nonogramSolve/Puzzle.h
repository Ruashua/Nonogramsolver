#pragma once

#include <string>
#include <ctime>
#include "tomography.h"
#include "Globals.h"

using namespace std;

class Puzzle
{
public:
	static int _width;
	static int _height;

	string* colors;
	string bgColor;

	int numberOfColors;

	short** theGrid;
	
	Tomography* tomographyWidth;
	Tomography* tomographyHeight;

	Puzzle();
	Puzzle(int w, int h, int c);
	~Puzzle();

	void printTheGrid();
	bool bruteForceValidity(short** theGrid, Tomography* tomographyWidth, Tomography* tomographyHeight, int i, int j, bool& tooLong, time_t& startTime);
};

