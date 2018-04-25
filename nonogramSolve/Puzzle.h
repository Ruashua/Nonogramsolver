#pragma once
//States of the grid 
//0 is blank
//1 is X'ed
//2+ are filled (+ for colors)
#include <string>
#include <ctime>
#include "tomography.h"
#include "Globals.h"

using namespace std;

class Puzzle
{
public:
	const int _width;
	const int _height;

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
	bool bruteForceValidity(int i, int j, bool& tooLong, time_t& startTime);
	void bruteForce();
};

