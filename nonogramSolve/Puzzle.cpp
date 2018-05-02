#include "Puzzle.h"


Puzzle::Puzzle() :_width(0), _height(0)	//DO NOT USE
{
	tomographyWidth = nullptr;
	tomographyHeight = nullptr;
	theGrid = nullptr;
	colors = nullptr;
	bgColor = "255 255 255 ";
	numberOfColors = 0;
}
Puzzle::Puzzle(int w, int h, int c):_width(w), _height(h)	//Initializes the puzzle and arrays with the width, height, and number of colors.  This should really be the only constructor used.
{

	if (c == 0)
	{
		numberOfColors = 1;
	}
	else
	{
		numberOfColors = c;
	}

	tomographyWidth = new Tomography(_width);
	tomographyHeight = new Tomography(_height);
	tomographyWidth->tomography = new Tomograph**[_width];
	tomographyHeight->tomography = new Tomograph**[_height];
	
	theGrid = new short*[_width];
	for (int i = 0; i < _width; i++)
	{
		theGrid[i] = new short[_height];
	}
	for (int i = 0; i < _width; i++)
	{
		for (int j = 0; j < _height; j++)
		{
			theGrid[i][j] = 0;
		}
	}

	colors = new string[numberOfColors];
	if (c == 0)
	{
		colors[0] = "0 0 0 ";
	}
}
Puzzle::~Puzzle()
{
	for (int i = 0; i < _width; i++)
	{
		delete[] theGrid[i];
	}
	delete[] theGrid;
	delete tomographyWidth;
	delete tomographyHeight;
	delete[] colors;
}

void Puzzle::printTheGrid()	//Prints grid to console
{
	std::system("cls");
	for (int l = 0; l < _height; l++)
	{
		for (int k = 0; k < _width; k++)
		{
			cout<<PRINTCHARS[theGrid[k][l]];
		}
		cout << endl;
	}
	cout << endl;
}

bool Puzzle::bruteForceValidity(int i, int j, bool& tooLong, time_t& startTime)	//Returns whether or not the current block is valid with the brute force algorithm
{
	int count;
	int tomographyLocation;

	tomographyLocation = 0;
	count = 0;
	for (int k = 0; k < _height; k++)
	{
		if (theGrid[i][k] != 0 && tomographyLocation < tomographyWidth->sizes[i] && tomographyWidth->tomography[i][tomographyLocation]->number == count && theGrid[i][k] - 1 != tomographyWidth->tomography[i][tomographyLocation]->color)
		{
			//The space must be filled, the tomography must not be completed, the filled section must be the length defined in the tomography, and it must have switched to a new color
			//Then it changes to the next tomography definition
			tomographyLocation++;
			count = 0;
			if (PRETTYPRINT)
			{
				if (!tooLong)
				{
					if (j == k)
					{
						printTheGrid();
						if (time(0) - startTime > PRINTTOOLONG)
						{
							tooLong = true;
							startTime = time(0);
						}
					}
				}
				else if (time(0) - startTime > PRINTTOOLONGINTERVAL)
				{
					printTheGrid();
					startTime = time(0);
				}
			}
		}
		if (tomographyLocation >= tomographyWidth->sizes[i]) //What has been filled is valid up to this point
		{
			if (theGrid[i][k] == 0) //You have reached the end of what has been filled
			{
				break;
			}
			if (theGrid[i][k] == 1) //This is a blank square, check the next square
			{
				continue;
			}
			if (theGrid[i][k] > 1) //But there is another filled space, must be invalid
			{
				return false;
			}
			
		}
		if (theGrid[i][k] > 1)  //If the space is filled
		{
			if (theGrid[i][k] - 1 == tomographyWidth->tomography[i][tomographyLocation]->color) //count blocks in a row of the same color
			{
				count++;
			}
			if (tomographyWidth->tomography[i][tomographyLocation]->number < count && theGrid[i][k] - 1 == tomographyWidth->tomography[i][tomographyLocation]->color) //row too long
			{
				return false;
			}
			if (theGrid[i][k] - 1 != tomographyWidth->tomography[i][tomographyLocation]->color && tomographyWidth->tomography[i][tomographyLocation]->number > count) //row is too small but switched to new color
			{
				return false;
			}
		}
		if (theGrid[i][k] == 1 && tomographyWidth->tomography[i][tomographyLocation]->number > count && count > 0) //Prevents breaks with gaps
		{
			return false;
		}
		
		
		
		
		/*if (theGrid[i][k] >= 2 && tomographyLocation < tomographyWidth->sizes[i] && tomographyWidth->tomography[i][tomographyLocation]->color == theGrid[i][k] - 1)
		{
			count++;
			if (tomographyLocation > tomographyWidth->sizes[i] - 1)
			{
				return false;
			}
			else
			{
				if (tomographyWidth->tomography[i][tomographyLocation]->number < count)
				{
					return false;
				}
			}
		}
		else
		{
			if (tomographyLocation < tomographyWidth->sizes[i])
			{
				if (tomographyWidth->tomography[i][tomographyLocation]->number > count && count > 0 && theGrid[i][k] != 0 && theGrid[i][k] != tomographyWidth->tomography[i][tomographyLocation]->color)
				{
					return false;
				}
				if (tomographyWidth->tomography[i][tomographyLocation]->number == count)
				{
					tomographyLocation++;
					count = 0;
					if (PRETTYPRINT)
					{
						if (!tooLong)
						{
							if (j == k)
							{
								printTheGrid();
								if (time(0) - startTime > 10)
								{
									tooLong = true;
									startTime = time(0);
								}
							}
						}
						else if (time(0) - startTime > 2)
						{
							printTheGrid();
							startTime = time(0);
						}
					}
				}
			}
		}*/
	}
	///*
	if (j == _height - 1)
	{
		if (tomographyLocation < tomographyWidth->sizes[i])
		{
			if (tomographyWidth->tomography[i][tomographyLocation]->number != count)
			{
				return false;
			}
			else if (tomographyLocation < tomographyWidth->sizes[i] - 1)
			{
				return false;
			}

			if (PRETTYPRINT)
			{
				if (!tooLong)
				{
					printTheGrid();
					if (time(0) - startTime > PRINTTOOLONG)
					{
						tooLong = true;
						startTime = time(0);
					}
				}
				else if (time(0) - startTime > PRINTTOOLONGINTERVAL)
				{
					printTheGrid();
					startTime = time(0);
				}
			}
		}
	}//*/


	tomographyLocation = 0;
	count = 0;
	
	for (int k = 0; k < _width; k++)
	{
		if (theGrid[k][j] != 0 && tomographyLocation < tomographyHeight->sizes[j] && tomographyHeight->tomography[j][tomographyLocation]->number == count && theGrid[k][j] - 1 != tomographyHeight->tomography[j][tomographyLocation]->color)
		{
			//The space must be filled, the tomography must not be completed, the filled section must be the length defined in the tomography, and it must have switched to a new color
			//Then it changes to the next tomography definition
			tomographyLocation++;
			count = 0;
		}
		if (tomographyLocation >= tomographyHeight->sizes[j]) //What has been filled is valid up to this point
		{
			if (theGrid[k][j] == 0) //You have reached the end of what has been filled
			{
				break;
			}
			if (theGrid[k][j] == 1) //This is a blank square, check the next square
			{
				continue;
			}
			if (theGrid[k][j] > 1) //But there is another filled space, must be invalid
			{
				return false;
			}

		}
		if (theGrid[k][j] > 1)  //If the space is filled
		{
			if (theGrid[k][j] - 1 == tomographyHeight->tomography[j][tomographyLocation]->color) //count blocks in a row of the same color
			{
				count++;
			}
			if (tomographyHeight->tomography[j][tomographyLocation]->number < count && theGrid[k][j] - 1 == tomographyHeight->tomography[j][tomographyLocation]->color) //row too long
			{
				return false;
			}
			if (theGrid[k][j] - 1 != tomographyHeight->tomography[j][tomographyLocation]->color && tomographyHeight->tomography[j][tomographyLocation]->number > count) //row is too small but switched to new color
			{
				return false;
			}
		}
		if (theGrid[k][j] == 1 && tomographyHeight->tomography[j][tomographyLocation]->number > count && count > 0) //Prevents breaks with gaps
		{
			return false;
		}
	}///*
	if (i == _width - 1)
	{
		if (tomographyLocation < tomographyHeight->sizes[j])
		{
			if (tomographyHeight->tomography[j][tomographyLocation]->number != count)
			{
				return false;
			}
		}

	}//*/
	return true;
}
void Puzzle::bruteForce()	//Solves the puzzle by placing and checking one block at a time starting at the top left, going along the columns.  Very inefficient.
{
	int i, j;


	bool valid = true;
	bool needBacktrack = false;
	i = 0;

	time_t startTime = time(0);
	bool tooLong = false;

	while (i < _width)
	{
		j = 0;
		while (j < _height)
		{


			if (needBacktrack && theGrid[i][j] == 1)
			{
				theGrid[i][j] = 0;
				needBacktrack = true;

				j = j - 2;
			}
			else
			{
				if (valid)
				{
					theGrid[i][j] = numberOfColors + 1;
				}
				valid = true;
				if (needBacktrack && theGrid[i][j] >= 2)
				{
					theGrid[i][j] = theGrid[i][j] - 1;
					needBacktrack = false;
				}
				if (DEBUG)
				{
					printTheGrid();
					//std::system("pause");
					//
				}

				valid = bruteForceValidity(i, j, tooLong, startTime);

				if (!valid)
				{
					if (theGrid[i][j] >= 2)
					{
						theGrid[i][j] = theGrid[i][j] - 1;
						j--;
					}
					else
					{
						theGrid[i][j] = 0;
						needBacktrack = true;
						j = j - 2;
					}
				}
			}

			j++;
			if (j < 0)
			{
				i--;
				j = _height - 1;
			}
			if (i < 0)
			{
				cout << "This one is impossible: ";
			}

		}
		i++;
	}
	cout << "DONE! ";
}

int Puzzle::calcNeededRoom(Tomography* tomography, int section, int startPosition)
{
	int total = 0;
	for (int i = startPosition; i < tomography->sizes[section]; i++)
	{
		total += tomography->tomography[section][i]->number;
		if (i != startPosition)
		{
			if (tomography->tomography[section][i]->color == tomography->tomography[section][i - 1]->color)
			{
				total++;
			}
		}
	}
	return total;
}
bool Puzzle::greedyValidity(int i, int j, bool& tooLong, time_t& startTime)  //TODO?: Check validity for greedy algorithm
{
	if (i == 2 && j == 4)
	{
		cout << "here";
	}
	int count;
	int tomographyLocation;

	tomographyLocation = 0;
	count = 0;

	for (int k = 0; k < i+1; k++)
	{
		if (tomographyLocation < tomographyHeight->sizes[j] && tomographyHeight->tomography[j][tomographyLocation]->number == count && theGrid[k][j] - 1 != tomographyHeight->tomography[j][tomographyLocation]->color)
		{
			//The space must be filled, the tomography must not be completed, the filled section must be the length defined in the tomography, and it must have switched to a new color
			//Then it changes to the next tomography definition
			tomographyLocation++;
			count = 0;
		}
		if (tomographyLocation >= tomographyHeight->sizes[j]) //What has been filled is valid up to this point
		{
			if (theGrid[k][j] == 0) //You have reached the end of what has been filled
			{
				continue;
			}
			if (theGrid[k][j] > 1) //But there is another filled space, must be invalid
			{
				return false;
			}

		}
		if (theGrid[k][j] > 1)  //If the space is filled
		{
			if (theGrid[k][j] - 1 == tomographyHeight->tomography[j][tomographyLocation]->color) //count blocks in a row of the same color
			{
				count++;
			}
			if (tomographyHeight->tomography[j][tomographyLocation]->number < count && theGrid[k][j] - 1 == tomographyHeight->tomography[j][tomographyLocation]->color) //row too long
			{
				return false;
			}
			if (theGrid[k][j] - 1 != tomographyHeight->tomography[j][tomographyLocation]->color && tomographyHeight->tomography[j][tomographyLocation]->number > count) //row is too small but switched to new color
			{
				return false;
			}
		}
		if (theGrid[k][j] == 0 && tomographyHeight->tomography[j][tomographyLocation]->number > count && count > 0) //Prevents breaks with gaps
		{
			return false;
		}
	}///*
	if (i == _width - 1)
	{
		if (tomographyLocation < tomographyHeight->sizes[j])
		{
			if (tomographyHeight->tomography[j][tomographyLocation]->number != count)
			{
				return false;
			}
		}

	}//*/
	return true;
}
bool Puzzle::greedyValidityFront(bool****& dpValidityGrid, int i, int j, int c, bool& tooLong, time_t& startTime)
{
	if (!dpValidityGrid[i][j][c][0])  //If validity of placement hasnt been calculated, calculate it
	{
		dpValidityGrid[i][j][c][1] = greedyValidity(i, j, tooLong, startTime);
		dpValidityGrid[i][j][c][0] = true;
	}
	return dpValidityGrid[i][j][c][1];
}
//bool placeGreedySection()

void Puzzle::greedy()  //OK....redo.... make it less efficient but more organized.
{
	int i, j, k, l, m, n; //counters
	bool needLoopThroughSections, needLoopThroughColumns, needToMakeSpaces, needCalcRoomNeeded, needBacktrack;
	int roomNeeded;
	int backTrackUndoAmount;
	time_t startTime = time(0);
	bool tooLong = false;

	bool**** dpValidityGrid;
	for (i = 0; i < _width; i++)
	{
		dpValidityGrid[i] = new bool**[_width];
		for (j = 0; j < _height; j++)
		{
			dpValidityGrid[i][j] = new bool*[numberOfColors + 1];
			for (k = 0; k < numberOfColors + 1; k++)
			{
				dpValidityGrid[i][j][k] = new bool[2];
				dpValidityGrid[i][j][k][0] = false;
				dpValidityGrid[i][j][k][1] = false;
			}
		}
	}

	i = 0;
	k = 0;
	needLoopThroughSections = true; 
	needLoopThroughColumns = true;
	needToMakeSpaces = false;
	while (needLoopThroughColumns)
	{
		//while (j<_height) //TODO loop through the height
		{
			j = 0;
			while (needLoopThroughSections) //TODO loop through sections/spaces
			{
				if (needToMakeSpaces)
				{

				}
				else
				{
					if (needCalcRoomNeeded) //If the minimum room needed hasnt been calculated, calculate it
					{
						roomNeeded = calcNeededRoom(tomographyWidth, i, k);
						needCalcRoomNeeded = false;
					}
					if ((_height - j) < roomNeeded)	//automatically needs to backtrack if there is no more room
					{
						needBacktrack = true;
						break; //TODO, need backtrack
					}
					j = tomographyWidth->tomography[i][k]->startPosition;
					for (l = j; l < tomographyWidth->tomography[i][k]->number + j; l++)  //Loop through the placement of the tomography section
					{
						theGrid[i][j] = short(tomographyWidth->tomography[i][k]->color);
						
						needBacktrack = greedyValidityFront(dpValidityGrid, i, l, tomographyWidth->tomography[i][k]->color, tooLong, startTime);

					}
				}

			}
		}
	}


	for (i = 0; i < _width; i++)
	{
		for (j = 0; j < _height; j++)
		{
			for (k = 0; k < numberOfColors + 1; k++)
			{
				delete[] dpValidityGrid[i][j][k];
			}
			delete[] dpValidityGrid[i][j];
		}
		delete[] dpValidityGrid[i];
	}
	delete[] dpValidityGrid;
}

/*void Puzzle::greedy()	//TODO: Solves puzzle one tomography element at a time.  Meh efficieny.  
{
	int i, j, k, l, m, n;
	bool needCalcRoomNeeded;
	bool needBacktrack;
	int roomNeeded;
	bool**** dpValidityGrid;  //This is a grid that keeps track of what validity has been evaluated and the validity. 
	bool sectionIsValid;
	int* tomographyStartPositions;
	//[width][height][color][0] = hasBeenEvaluated?  
	//[width][height][color][1] = isValid?
	//[width][height][0] = blankSpace
	//[width][height][1] = X'ed
	//[width][height][>1] = theActualColors
	short temp = 0;
	dpValidityGrid = new bool***[_width];
	for (i = 0; i < _width; i++)
	{
		dpValidityGrid[i] = new bool**[_width];
		for (j = 0; j < _height; j++)
		{
			dpValidityGrid[i][j] = new bool*[numberOfColors + 2];
			for (k = 0; k < numberOfColors + 2; k++)
			{
				dpValidityGrid[i][j][k] = new bool[2];
				dpValidityGrid[i][j][k][0] = false;
				dpValidityGrid[i][j][k][1] = false;
			}
		}
	}
	time_t startTime = time(0);
	bool tooLong = false;
	needBacktrack = false;
	i = 0;
	while (i < _width)
	{
		if (!needBacktrack)
		{
			j = 0;
			k = 0;
		}
		else if (j >= _height)
		{
			if (needBacktrack)  //Go to the last section
			{
				needCalcRoomNeeded = true;
				k--;
				if (k < 0)  //If backtracking passed the first section of the row
				{ //TODO
					for (m = 0; m < _height; m++) //delete the row and all of its validity
					{
						theGrid[i][m] = 0;
						for (n = 0; n < numberOfColors + 2; n++)
						{
							dpValidityGrid[i][m][n][0] = false;
							//dpValidityGrid[i][m][n][1] = false;
						}
					}
					i--;
					k = tomographyWidth->sizes[i] - 1;
					for (m = tomographyWidth->tomography[i][k]->startPosition; m < tomographyWidth->tomography[i][k]->startPosition + tomographyWidth->tomography[i][k]->number; m++)
					{
						theGrid[i][m] = 0;
					}
					
				}
				else
				{
					for (m = tomographyWidth->tomography[i][k]->startPosition; m < tomographyWidth->tomography[i][k]->startPosition + tomographyWidth->tomography[i][k]->number; m++)
					{
						theGrid[i][m] = 0; //TODO
					}
				}
				needBacktrack = false;
				if (DEBUG)
				{
					printTheGrid();
					//std::system("pause");
				}
			}
			//j = _height - 1;
			j = tomographyWidth->tomography[i][k]->startPosition + 1;
			if (!dpValidityGrid[i][j - 1][0][0])
			{
				dpValidityGrid[i][j - 1][0][1] = greedyValidity(i, j - 1, tooLong, startTime);
				dpValidityGrid[i][j - 1][0][0] = true;
			}
			if (!dpValidityGrid[i][j - 1][0][1])
			{
				needBacktrack = true;
				continue;
			}
			if (j >= _height)
			{
				i--;
			}
		}
		needCalcRoomNeeded = true;
		//tomographyStartPositions = new int[tomographyWidth->sizes[i]];
		if (i == 3)
		{
			cout << "here";
		}
		while (j < _height)
		{
			if (theGrid[0][2] == 2 && theGrid[0][4] == 2 && theGrid[1][0] == 0)
			{

			}

			if (k < tomographyWidth->sizes[i] && !needBacktrack)
			{
				tomographyWidth->tomography[i][k]->startPosition = j;
				sectionIsValid = true;
				for (l = 0; l < tomographyWidth->tomography[i][k]->number; l++)  //Loop through the placement of the tomography section
				{
					//cout << (_height - j) << " " << calcNeededRoom(tomographyWidth, i, k);
					//std::system("pause");

					if (needCalcRoomNeeded) //If the minimum room needed hasnt been calculated, calculate it
					{
						roomNeeded = calcNeededRoom(tomographyWidth, i, k);
						needCalcRoomNeeded = false;
					}
					if ((_height - j) < roomNeeded)	//automatically needs to backtrack if there is no more room
					{
						needBacktrack = true;
						break; //TODO, need backtrack
					}
					theGrid[i][j + l] = short(tomographyWidth->tomography[i][k]->color + 1);
					if (DEBUG)
					{
						printTheGrid();
						//std::system("pause");
					}
					//TODO check if valid, store in valid array
					if (!dpValidityGrid[i][j + l][tomographyWidth->tomography[i][k]->color + 1][0])  //If validity of placement hasnt been calculated, calculate it
					{
						dpValidityGrid[i][j + l][tomographyWidth->tomography[i][k]->color + 1][1] = greedyValidity(i, j + l, tooLong, startTime);
						dpValidityGrid[i][j + l][tomographyWidth->tomography[i][k]->color + 1][0] = true;
					}

					if (!dpValidityGrid[i][j + l][tomographyWidth->tomography[i][k]->color + 1][1])  //If any placement is not valid, undo what was done, increment the starting location by 1 and start again
					{
						sectionIsValid = false;
						for (m = j; m == j + l; m++)
						{
							theGrid[i][m] = 0;  //TODO
							if (!dpValidityGrid[i][m][0][0])
							{
								dpValidityGrid[i][m][0][1] = greedyValidity(i, m, tooLong, startTime);
								dpValidityGrid[i][m][0][0] = true;
							}
							if (!dpValidityGrid[i][m][0][1])
							{
								needBacktrack = true;
								break;
							}
						}

						j++;
						break;
					}
				}
				if (!needBacktrack && sectionIsValid)  //If a valid section is done being made, change the loop incrementor
				{
					j += l;
					k++;
					needCalcRoomNeeded = true;
					if (k < tomographyWidth->sizes[i])  //If you are not past the last section
					{
						if (tomographyWidth->tomography[i][k]->color == tomographyWidth->tomography[i][k - 1]->color)  //If the color of the next section is the same as the last, you need a space
						{
							theGrid[i][j] = 0;
							if (!dpValidityGrid[i][j][0][0])  //Check if that space is valid
							{
								dpValidityGrid[i][j][0][1] = greedyValidity(i, j, tooLong, startTime);
								dpValidityGrid[i][j][0][0] = true;
							}
							if (!dpValidityGrid[i][j][0][1]) //if it is not valid, you need to increment the last section
							{
								needBacktrack = true;
							}
							j++;
						}
					}
				}
			}
			else if (!needBacktrack)
			{
				for (l = j; l < _height; l++)  //Validate the spaces after placing every section
				{
					if (!dpValidityGrid[i][l][0][0])  //Check if that space is valid
					{
						dpValidityGrid[i][l][0][1] = greedyValidity(i, l, tooLong, startTime);
						dpValidityGrid[i][l][0][0] = true;
					}
					if (!dpValidityGrid[i][l][0][1]) //if it is not valid, this is the new starting position for the last section.
					{
						needBacktrack = true;
						j = l - tomographyWidth->tomography[i][k-1]->number + 1;
						break;
					}
					else
					{
						j = _height;
					}
				}
				
			}

			if (needBacktrack)  //Go to the last section
			{
				needCalcRoomNeeded = true;
				k--;
				if (k < 0)  //If backtracking passed the first section of the row
				{ //TODO
					for (m = 0; m < _height; m++) //delete the row and all of its validity
					{
						theGrid[i][m] = 0;
						for (n = 0; n < numberOfColors + 2; n++)
						{
							dpValidityGrid[i][m][n][0] = false;
							//dpValidityGrid[i][m][n][1] = false;
						}
					}
					i--;
					k = tomographyWidth->sizes[i] - 1;
					for (m = tomographyWidth->tomography[i][k]->startPosition; m < tomographyWidth->tomography[i][k]->startPosition + tomographyWidth->tomography[i][k]->number; m++)
					{
						theGrid[i][m] = 0;
					}
					j = tomographyWidth->tomography[i][k]->startPosition + 1;  
					if (j >= _height)
					{
						needBacktrack = true;
						i--;
						continue;
					}
					if (!dpValidityGrid[i][j - 1][0][0])
					{
						dpValidityGrid[i][j - 1][0][1] = greedyValidity(i, j - 1, tooLong, startTime);
						dpValidityGrid[i][j - 1][0][0] = true;
					}
					if (!dpValidityGrid[i][j - 1][0][1])
					{
						needBacktrack = true;
						continue;
					}
				}
				else
				{
					for (m = tomographyWidth->tomography[i][k]->startPosition; m < tomographyWidth->tomography[i][k]->startPosition + tomographyWidth->tomography[i][k]->number; m++)
					{
						theGrid[i][m] = 0; //TODO
					}
				}
				
				
				if (j  < _height)
				{
					needBacktrack = false;
				}
				else
				{
					i--;
				}

				if (DEBUG)
				{
					printTheGrid();
					//std::system("pause");
				}
				continue;
			}
			
		}

		i++;
	}


	for (i = 0; i < _width; i++)
	{
		for (j = 0; j < _height; j++)
		{
			for (k = 0; k < numberOfColors + 2; k++)
			{
				delete[] dpValidityGrid[i][j][k];
			}
			delete[] dpValidityGrid[i][j];
		}
		delete[] dpValidityGrid[i];
	}
	delete[] dpValidityGrid;
}*/

/*void Puzzle::greedy()	//TODO: Solves puzzle one tomography element at a time.  Meh efficieny.  
{
	int i, j, k, l, m, n;
	bool needCalcRoomNeeded;
	bool needBacktrack;
	int roomNeeded;
	bool**** dpValidityGrid;  //This is a grid that keeps track of what validity has been evaluated and the validity. 
	int* tomographyStartPositions;
	//[width][height][color][0] = hasBeenEvaluated?  
	//[width][height][color][1] = isValid?
	//[width][height][0] = blankSpace
	//[width][height][1] = X'ed
	//[width][height][>1] = theActualColors
	short temp = 0;
	dpValidityGrid = new bool***[_width];
	for (i = 0; i < _width; i++)
	{
		dpValidityGrid[i] = new bool**[_width];
		for (j = 0; j < _height; j++)
		{
			dpValidityGrid[i][j] = new bool*[numberOfColors + 2];
			for (k = 0; k < numberOfColors + 2; k++)
			{
				dpValidityGrid[i][j][k] = new bool[2];
				dpValidityGrid[i][j][k][0] = false;
				dpValidityGrid[i][j][k][1] = false;
			}
		}
	}
	time_t startTime = time(0);
	bool tooLong = false;
	needBacktrack = false;
	i = 0;
	while (i < _width)
	{
		if (!needBacktrack)
		{
			j = 0;
			k = 0;
		}
		else if (j >= _height)
		{
			j = _height - 1;
		}
		needCalcRoomNeeded = true;
		//tomographyStartPositions = new int[tomographyWidth->sizes[i]];
		if (i == 3)
		{
			cout << "here";
		}
		while (j < _height && k < tomographyWidth->sizes[i])
		{

			tomographyWidth->tomography[i][k]->startPosition = j;
			if (!needBacktrack)
			{
				for (l = 0; l < tomographyWidth->tomography[i][k]->number; l++)
				{
					//cout << (_height - j) << " " << calcNeededRoom(tomographyWidth, i, k);
					//std::system("pause");

					if (needCalcRoomNeeded)
					{
						roomNeeded = calcNeededRoom(tomographyWidth, i, k);
						needCalcRoomNeeded = false;
					}
					if ((_height - j) < roomNeeded)	//automatically invalid if there is no more room
					{
						needBacktrack = true;
						break; //TODO, need backtrack
					}
					theGrid[i][j + l] = short(tomographyWidth->tomography[i][k]->color + 1);
					if (DEBUG)
					{
						printTheGrid();
						//std::system("pause");
					}
					//TODO check if valid, store in valid array
					if (!dpValidityGrid[i][j + l][tomographyWidth->tomography[i][k]->color + 1][0])
					{
						dpValidityGrid[i][j + l][tomographyWidth->tomography[i][k]->color + 1][1] = greedyValidity(i, j + l, tooLong, startTime);
						dpValidityGrid[i][j + l][tomographyWidth->tomography[i][k]->color + 1][0] = true;
					}

					if (!dpValidityGrid[i][j + l][tomographyWidth->tomography[i][k]->color + 1][1])
					{
						for (m = j; m == j + l; m++)
						{
							theGrid[i][m] = 1;  //TODO
							if (!dpValidityGrid[i][m][1][0])
							{
								dpValidityGrid[i][m][1][1] = greedyValidity(i, m, tooLong, startTime);
								dpValidityGrid[i][m][1][0] = true;
							}
							if (!dpValidityGrid[i][m][1][1])
							{
								needBacktrack = true;
							}
						}

						j++;
						break;
					}


				}
			}
			if (needBacktrack)
			{
				needCalcRoomNeeded = true;
				k--;
				if (k < 0)
				{ //TODO
					for (m = 0; m < _height; m++)
					{
						theGrid[i][m] = 0;
						for (n = 0; n < numberOfColors + 2; n++)
						{
							dpValidityGrid[i][m][n][0] = false;
							dpValidityGrid[i][m][n][1] = false;
						}
					}
					i--;
					k = tomographyWidth->sizes[i] - 1;
					for (m = tomographyWidth->tomography[i][k]->startPosition; m < tomographyWidth->tomography[i][k]->startPosition + tomographyWidth->tomography[i][k]->number; m++)
					{
						theGrid[i][m] = 0;
					}
				}
				else
				{
					for (m = tomographyWidth->tomography[i][k]->startPosition; m < tomographyWidth->tomography[i][k]->startPosition + tomographyWidth->tomography[i][k]->number; m++)
					{
						theGrid[i][m] = 0;
					}
				}
				j = tomographyWidth->tomography[i][k]->startPosition + 1;
				theGrid[i][j - 1] = 1;  //TODO
				if (!dpValidityGrid[i][j - 1][1][0])
				{
					dpValidityGrid[i][j - 1][1][1] = greedyValidity(i, j - 1, tooLong, startTime);
					dpValidityGrid[i][j - 1][1][0] = true;
				}
				if (!dpValidityGrid[i][j - 1][1][1])
				{
					needBacktrack = true;
					continue;
				}
				if (j < _height)
				{
					needBacktrack = false;
				}
				else
				{
					i--;
				}

				if (DEBUG)
				{
					printTheGrid();
					//std::system("pause");
				}
				continue;
			}
			if (dpValidityGrid[i][j][tomographyWidth->tomography[i][k]->color + 1][1])
			{
				j += l;
				k++;
				needCalcRoomNeeded = true;
				if (k < tomographyWidth->sizes[i] && k>0)
				{
					if (tomographyWidth->tomography[i][k]->color == tomographyWidth->tomography[i][k - 1]->color)
					{
						theGrid[i][j] = 1;
						if (!dpValidityGrid[i][j][1][0])
						{
							dpValidityGrid[i][j][1][1] = greedyValidity(i, j, tooLong, startTime);
							dpValidityGrid[i][j][1][0] = true;
						}
						if (!dpValidityGrid[i][j][1][1])
						{
							needBacktrack = true;
						}
						j++;
					}
				}
			}
			else
			{
				j++;
			}
		}

		i++;
	}


	for (i = 0; i < _width; i++)
	{
		for (j = 0; j < _height; j++)
		{
			for (k = 0; k < numberOfColors + 2; k++)
			{
				delete[] dpValidityGrid[i][j][k];
			}
			delete[] dpValidityGrid[i][j];
		}
		delete[] dpValidityGrid[i];
	}
	delete[] dpValidityGrid;
}*/