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
Puzzle::Puzzle(const Puzzle& obj)
{
	numberOfColors = obj.numberOfColors;
	_height = obj._height;
	_width = obj._width;
	bgColor = obj.bgColor;

	colors = new string[numberOfColors];
	for (int i = 0; i < numberOfColors; i++)
	{
		colors[i] = obj.colors[i];
	}

	theGrid = new short*[_width];
	for (int i = 0; i < _width; i++)
	{
		theGrid[i] = new short[_height];
		for (int j = 0; j < _height; j++)
		{
			theGrid[i][j] = obj.theGrid[i][j];
		}
	}

	tomographyWidth = new Tomography(*obj.tomographyWidth);
	tomographyHeight = new Tomography(*obj.tomographyHeight);
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

// Returns value of Binomial Coefficient C(n, k)
unsigned long long binomialCoeff(int n, int k)
{
	unsigned long long res = 1;

	// Since C(n, k) = C(n, n-k)
	if (k > n - k)
		k = n - k;

	// Calculate value of [n * (n-1) *---* (n-k+1)] / [k * (k-1) *----* 1]
	for (int i = 0; i < k; ++i)
	{
		res *= (n - i);
		res /= (i + 1);
	}

	return res;
}


void Puzzle::zeroTheGrid()
{
	for (int l = 0; l < _height; l++)
	{
		for (int k = 0; k < _width; k++)
		{
			theGrid[k][l] = 0;
		}
	}
}

void Puzzle::printTheGrid(int column)	//Prints grid to console up to a column
{
	std::system("cls");
	for (int l = 0; l < _height; l++)
	{
		for (int k = 0; k < column; k++)
		{
			cout << PRINTCHARS[theGrid[k][l]];
		}
		cout << endl;
	}
	cout << endl;
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

void Puzzle::transposePuzzle()  //Swaps width and height
{
	Tomography* t;
	t = tomographyHeight;
	tomographyHeight = tomographyWidth;
	tomographyWidth = t;

	short** tempGrid;
	tempGrid = new short*[_height];
	for (int i = 0; i < _height; i++)
	{
		tempGrid[i] = new short[_width];
		for (int j = 0; j < _width; j++)
		{
			tempGrid[i][j] = theGrid[j][i];
		}
	}
	//zeroTheGrid();

	for (int i = 0; i < _width; i++)
	{
		delete[] theGrid[i];
	}
	delete[] theGrid;

	int i;
	i = _width;
	_width = _height;
	_height = i;

	theGrid = tempGrid;
}

void Puzzle::mirrorPuzzle(Tomography*& tomographyMirrorAxis, Tomography*& tomographyOtherAxis, bool overWidth)  //Mirrors puzzle over an axis. So if width is first variable, it mirrors the width.
{
	Tomography* mirrorAxis = new Tomography(tomographyMirrorAxis->dimensionSize);
	Tomography* otherAxis = new Tomography(tomographyOtherAxis->dimensionSize);

	for (int i = tomographyMirrorAxis->dimensionSize - 1; i >= 0; i--)
	{
		mirrorAxis->sizes[tomographyMirrorAxis->dimensionSize - 1 - i] = tomographyMirrorAxis->sizes[i];
		
		if (tomographyMirrorAxis->sizes[i] == 0)
		{
			mirrorAxis->tomography[tomographyMirrorAxis->dimensionSize - 1 - i] = new Tomograph*[1];
			mirrorAxis->tomography[tomographyMirrorAxis->dimensionSize - 1 - i][0] = new Tomograph(tomographyMirrorAxis->tomography[i][0]->number, tomographyMirrorAxis->tomography[i][0]->color);
		}
		else
		{
			mirrorAxis->tomography[tomographyMirrorAxis->dimensionSize - 1 - i] = new Tomograph*[tomographyMirrorAxis->sizes[i]];
		}
		for (int j = 0; j < tomographyMirrorAxis->sizes[i]; j++)
		{
			mirrorAxis->tomography[tomographyMirrorAxis->dimensionSize - 1 - i][j] = new Tomograph(tomographyMirrorAxis->tomography[i][j]->number, tomographyMirrorAxis->tomography[i][j]->color);
			
		}
	}

	delete tomographyMirrorAxis;
	tomographyMirrorAxis = mirrorAxis;

	for (int i = 0; i < tomographyOtherAxis->dimensionSize; i++)
	{
		otherAxis->sizes[i] = tomographyOtherAxis->sizes[i];
		if (otherAxis->sizes[i] == 0)
		{
			otherAxis->tomography[i] = new Tomograph*[1];
			otherAxis->tomography[i][0] = new Tomograph(tomographyOtherAxis->tomography[i][0]->number, tomographyOtherAxis->tomography[i][0]->color);
		}
		else
		{
			
			otherAxis->tomography[i] = new Tomograph*[tomographyOtherAxis->sizes[i]];
		}

		for (int j = tomographyOtherAxis->sizes[i] - 1; j >=0; j--)
		{
			otherAxis->tomography[i][tomographyOtherAxis->sizes[i] - 1 - j] = new Tomograph(tomographyOtherAxis->tomography[i][j]->number, tomographyOtherAxis->tomography[i][j]->color);
		}
	}

	delete tomographyOtherAxis;
	tomographyOtherAxis = otherAxis;

	short temp;
	if (overWidth)
	{
		for (int i = 0; i < _width / 2; i++)
		{
			for (int j = 0; j < _height; j++)
			{
				temp = theGrid[i][j];
				theGrid[i][j] = theGrid[_width - i - 1][j];
				theGrid[_width - i - 1][j] = temp;
			}
		}
	}
	else
	{
		for (int i = 0; i < _width; i++)
		{
			for (int j = 0; j < _height / 2; j++)
			{
				temp = theGrid[i][j];
				theGrid[i][j] = theGrid[i][_height - j - 1];
				theGrid[i][_height - j - 1] = temp;
			}
		}
	}
}
void Puzzle::transOrMirrorForParallel(int number)
{
	if ((number & 1) == 1)
	{
		mirrorPuzzle(tomographyWidth, tomographyHeight, true);
	}
	if (((number >> 1) & 1) == 1)
	{
		mirrorPuzzle(tomographyHeight, tomographyWidth, false);
	}
	if (((number >> 2) & 1) == 1)
	{
		transposePuzzle();
	}
}
void Puzzle::undoTransOrMirrorForParallel(int number)
{
	if (((number >> 2) & 1) == 1)
	{
		transposePuzzle();
	}
	if (((number >> 1) & 1) == 1)
	{
		mirrorPuzzle(tomographyHeight, tomographyWidth, false);
	}
	if ((number & 1) == 1)
	{
		mirrorPuzzle(tomographyWidth, tomographyHeight, true);
	}
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
#ifdef PRETTYPRINT	
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
#endif // PRETTYPRINT
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
#ifdef PRETTYPRINT
			
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
#endif // PRETTYPRINT
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
#ifdef DEBUG
				printTheGrid();
				//std::system("pause");
#endif //DEBUG

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
bool Puzzle::greedyValidity(int i, int j)  //TODO?: Check validity for greedy algorithm
{
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
bool Puzzle::greedyValidityFront(bool****& dpValidityGrid, int i, int j, int c)
{
	if (!dpValidityGrid[i][j][c][0])  //If validity of placement hasnt been calculated, calculate it
	{
		dpValidityGrid[i][j][c][1] = greedyValidity(i, j);
		dpValidityGrid[i][j][c][0] = true;
	}
	return !dpValidityGrid[i][j][c][1];
}

bool Puzzle::greedy()  //OK....redo.... make it less efficient but more organized.
{
	int i, j, k, l, m; //counters
	bool needLoopThroughSections, needLoopThroughColumns, needToMakeSpaces, needCalcRoomNeeded, needBacktrack, needBacktrackLastOne;
	int roomNeeded;
	int backTrackUndoAmount, backTrackJumpForwardAmount;
	int spacesNeeded;
	time_t startTime = time(0);
	bool tooLong = false;

	bool**** dpValidityGrid;
	dpValidityGrid = new bool***[_width];
	for (i = 0; i < _width; i++)
	{
		dpValidityGrid[i] = new bool**[_height];
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
	j = 0;
	spacesNeeded = 0;
	backTrackUndoAmount = 0;
	backTrackJumpForwardAmount = 0;
	needLoopThroughSections = true; 
	needLoopThroughColumns = true;
	needToMakeSpaces = false;
	needBacktrack = false;
	needCalcRoomNeeded = true;
	needBacktrackLastOne = false;

			
	while (needLoopThroughSections && RACENOTFINISHED) //TODO loop through sections/spaces
	{
		if (needBacktrackLastOne)
		{
			k--;
			if (k < 0)
			{
				for (l = 0; l < _height; l++)
				{
					for (m = 0; m < numberOfColors + 1; m++)
					{
						dpValidityGrid[i][l][m][0] = false;
					}
				}
				i--;
				k = tomographyWidth->sizes[i] - 1;
				if (tomographyWidth->tomography[i][0]->number == 0)
				{
					continue;
				}

			}
			j = tomographyWidth->tomography[i][k]->startPosition;
			needBacktrackLastOne = false;
			needCalcRoomNeeded = true;
			backTrackUndoAmount = tomographyWidth->tomography[i][k]->number;
			backTrackJumpForwardAmount = 1;
			continue;
		}
		else if (needBacktrack)
		{
					
			for (l = j; l < j + backTrackUndoAmount; l++)
			{
				theGrid[i][l] = 0;
#ifdef DEBUG
				printTheGrid();
					//std::system("pause");
#endif //DEBUG
			}
			for (l = j; l < j + backTrackJumpForwardAmount; l++)
			{
				needBacktrack = greedyValidityFront(dpValidityGrid, i, l, 0);
				if (needBacktrack)
				{
					needBacktrackLastOne = true;
					break;
				}
			}
			if (needBacktrack)
			{
				continue;
			}
			j += backTrackJumpForwardAmount;
					
			if (j >= _height)
			{
				needBacktrackLastOne = true;
				needBacktrack = true;
				continue;
			}

			backTrackUndoAmount = 0;
			backTrackJumpForwardAmount = 0;
			needBacktrack = false;
			continue;
		}
		else if (needToMakeSpaces)
		{
			for (l = 0; l < spacesNeeded; l++)  //Validate "inserted" spaces
			{
				needBacktrack = greedyValidityFront(dpValidityGrid, i, j+l, 0);
				if (needBacktrack)
				{
					j = tomographyWidth->tomography[i][k]->startPosition;
					backTrackUndoAmount = tomographyWidth->tomography[i][k]->number;
					backTrackJumpForwardAmount = 1;
					break;
				}
			}
			if (!needBacktrack)
			{ 
				j += spacesNeeded;
				k++;
				needCalcRoomNeeded = true;
			}
			needToMakeSpaces = false;
		}
		else
		{
			if (tomographyWidth->tomography[i][k]->number == 0)
			{
				i++;
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
					needBacktrackLastOne = true;
					continue; //TODO, need backtrack
				}
				tomographyWidth->tomography[i][k]->startPosition = j;
				for (l = j; l < tomographyWidth->tomography[i][k]->number + j; l++)  //Loop through the placement of the tomography section
				{
					theGrid[i][l] = short(tomographyWidth->tomography[i][k]->color + 1);
#ifdef DEBUG	
					printTheGrid();
					//std::system("pause");		
#endif
					needBacktrack = greedyValidityFront(dpValidityGrid, i, l, tomographyWidth->tomography[i][k]->color);
					if (needBacktrack)
					{
						backTrackUndoAmount = l + 1 - j;
						backTrackJumpForwardAmount = l + 1 - j;
						break;
					}
				}
				if (!needBacktrack)
				{
#ifdef PRETTYPRINT
					if (!tooLong)
					{
						printTheGrid(i + 1);
						if (time(0) - startTime > PRINTTOOLONG)
						{
							tooLong = true;
							startTime = time(0);
						}
					}
					else if (time(0) - startTime > PRINTTOOLONGINTERVAL)
					{
						printTheGrid(i + 1);
						startTime = time(0);
					}
#endif // PRETTYPRINT
					j = l;
					if (k < tomographyWidth->sizes[i] - 1)
					{
						if (tomographyWidth->tomography[i][k]->color == tomographyWidth->tomography[i][k + 1]->color)
						{
							needToMakeSpaces = true;
							spacesNeeded = 1;
						}
						else
						{
							needToMakeSpaces = true;
							spacesNeeded = 0;
						}
					}
					else if (k == tomographyWidth->sizes[i] - 1)
					{
						if (j != _height)
						{
							needToMakeSpaces = true;
							spacesNeeded = _height - j;
						}
					}
				}
			}
		}
		if (j >= _height)
		{
			i++;
			j = 0;
			k = 0;
			needCalcRoomNeeded = true;
		}
		if (i >= _width)
		{
			needLoopThroughSections = false;
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

	if (!RACENOTFINISHED)
	{
		return false;
	}
	else
	{
		RACENOTFINISHED = false;
		return true;
	}
}

unsigned long long* Puzzle::tomographyHeat(Tomography*& tomographyAxis)
{
	unsigned long long* heat;
	int slots, items;

	heat = new unsigned long long[tomographyAxis->dimensionSize];

	for (int i = 0; i < tomographyAxis->dimensionSize; i++)
	{
		items = tomographyAxis->dimensionSize - calcNeededRoom(tomographyAxis, i, 0);
		slots = tomographyAxis->sizes[i] + 1;
		heat[i] = binomialCoeff(slots + items - 1, items);
		//cout << heat[i] <<" ";
	}
	//cout << endl;

	return heat;
}

void Puzzle::placeTrivial()  //TODO
{

}

void Puzzle::likeHuman()  //TODO
{

}