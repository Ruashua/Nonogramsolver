#include "Puzzle.h"


Puzzle::Puzzle()
{
	tomographyWidth = nullptr;
	tomographyHeight = nullptr;
	theGrid = nullptr;
	_height = 0;
	_width = 0;
	colors = nullptr;
	bgColor = "255 255 255 ";
	numberOfColors = 0;
}
Puzzle::Puzzle(int w, int h, int c)
{
	_width = w;
	_height = h;
	if (c == 0)
	{
		numberOfColors = 1;
	}
	else
	{
		numberOfColors = c;
	}

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

void Puzzle::printTheGrid()
{
	std::system("cls");
	for (int l = 0; l < _height; l++)
	{
		for (int k = 0; k < _width; k++)
		{
			switch (theGrid[k][l])
			{
			case 1:
				cout << "X";
				break;
			case 2:
				cout << char(178);
				break;
			}
		}
		cout << endl;
	}
	cout << endl;
}

bool Puzzle::bruteForceValidity(short** theGrid, Tomography* tomographyWidth, Tomography* tomographyHeight, int i, int j, bool& tooLong, time_t& startTime)
{
	int count;
	int tomographyLocation;

	tomographyLocation = 0;
	count = 0;
	for (int k = 0; k < _height; k++)
	{
		if (theGrid[i][k] == 2)
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
				if (tomographyWidth->tomography[i][tomographyLocation]->number > count && count > 0 && theGrid[i][k] == 1)
				{
					return false;
				}
				//std::cout << tomographyWidth->tomography[i][tomographyLocation]->number << " " << count << endl;
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
		}
	}
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
					if (time(0) - startTime > 10)
					{
						tooLong = true;
						startTime = time(0);
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


	tomographyLocation = 0;
	count = 0;
	for (int k = 0; k < _width; k++)
	{
		if (theGrid[k][j] == 2)
		{
			count++;
			if (tomographyLocation > tomographyHeight->sizes[j] - 1)
			{
				return false;
			}
			else
			{
				if (tomographyHeight->tomography[j][tomographyLocation]->number < count)
				{
					return false;
				}
			}
		}
		else
		{
			if (tomographyLocation < tomographyHeight->sizes[j])
			{
				if (tomographyHeight->tomography[j][tomographyLocation]->number > count && count > 0 && theGrid[k][j] == 1)
				{
					return false;
				}
				//std::cout << tomographyHeight->tomography[j][tomographyLocation]->number << " " << count << endl;
				if (tomographyHeight->tomography[j][tomographyLocation]->number == count)
				{
					tomographyLocation++;
					count = 0;
				}
			}
		}
	}
	if (i == _width - 1)
	{
		if (tomographyLocation < tomographyHeight->sizes[j])
		{
			if (tomographyHeight->tomography[j][tomographyLocation]->number != count)
			{
				return false;
			}
		}

	}
	return true;
}