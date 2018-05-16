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


class Puzzle
{
public:
	int _width;	//Width dimension of the puzzle
	int _height;	//Height dimension of the puzzle

	int numberOfColors;  //Size of the colors string array
	string* colors;		//Colors array, stores all of the colors in the puzzle
	string bgColor;		//The background color of the puzzle, most of the time it is white

	short** theGrid;	//The puzzle's....play area?
	
	Tomography* tomographyWidth;	//The tomography along the top, spanning the width
	Tomography* tomographyHeight;	//The tomography on the side, spanning the height

	Puzzle();	//DO NOT USE
	Puzzle(const Puzzle& obj);
	Puzzle(int w, int h, int c);	//Initializes the puzzle and arrays with the width, height, and number of colors.  This should really be the only constructor used.
	~Puzzle();

	void Puzzle::zeroTheGrid();
	void Puzzle::printTheGrid(int column); //Prints grid to console up to a column
	void printTheGrid();	//Prints grid to console
	void Puzzle::transposePuzzle();  //Swaps width and height (resets theGrid to zero)
	void Puzzle::mirrorPuzzle(Tomography*& tomographyMirrorAxis, Tomography*& tomographyOtherAxis, bool overWidth);  //Mirrors puzzle over an axis. So if width is first variable, it mirrors the width.
	void Puzzle::transOrMirrorForParallel(int number);
	void Puzzle::undoTransOrMirrorForParallel(int number);

	bool bruteForceValidity(int i, int j, bool& tooLong, time_t& startTime);	//Returns whether or not the current block is valid with the brute force algorithm
	void bruteForce();	//Solves the puzzle by placing and checking one block at a time starting at the top left, going along the columns.  Very inefficient.

	int Puzzle::calcNeededRoom(Tomography* tomography, int section, int startPosition);

	bool Puzzle::greedyValidity(int i, int j);
	bool Puzzle::greedyValidityFront(bool****& dpValidityGrid, int i, int j, int c);
	bool Puzzle::greedy();	//Solves puzzle one tomography element at a time.  Meh efficieny.  Returns if it finished (parallel only)

	unsigned long long* Puzzle::tomographyHeat(Tomography*& tomographyAxis);

	void Puzzle::placeTrivial();
	void Puzzle::likeHuman();
};

