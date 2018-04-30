#pragma once
//States of the grid 
//0 is blank
//1 is X'ed
//2+ are filled (+ for colors)
#include <iostream>
#include <string>
#include <ctime>
#include "tomography.h"
#include "Globals.h"

using namespace std;

class Puzzle
{
public:
	const int _width;	//Width dimension of the puzzle
	const int _height;	//Height dimension of the puzzle

	int numberOfColors;  //Size of the colors string array
	string* colors;		//Colors array, stores all of the colors in the puzzle
	string bgColor;		//The background color of the puzzle, most of the time it is white

	short** theGrid;	//The puzzle's....play area?
	
	Tomography* tomographyWidth;	//The tomography along the top, spanning the width
	Tomography* tomographyHeight;	//The tomography on the side, spanning the height

	Puzzle();	//DO NOT USE
	Puzzle(int w, int h, int c);	//Initializes the puzzle and arrays with the width, height, and number of colors.  This should really be the only constructor used.
	~Puzzle();

	void printTheGrid();	//Prints grid to console

	bool bruteForceValidity(int i, int j, bool& tooLong, time_t& startTime);	//Returns whether or not the current block is valid with the brute force algorithm
	void bruteForce();	//Solves the puzzle by placing and checking one block at a time starting at the top left, going along the columns.  Very inefficient.

	bool Puzzle::greedyValidity(int i, int j, bool& tooLong, time_t& startTime);
	void Puzzle::greedy();	//Solves puzzle one tomography element at a time.  Meh efficieny.  
};

