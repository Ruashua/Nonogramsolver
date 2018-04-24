#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <ctime>
#include "Puzzle.h"
#include "Globals.h"

using namespace std;

//States of the grid 
//0 is blank
//1 is X'ed
//2+ are filled (+ for colors)



string colorHexToRgb(string hexColor)
{
	int colorPart;
	string outColor;
	stringstream ssColor(hexColor);
	ssColor >> std::hex >> colorPart;
	outColor = std::to_string(((colorPart >> 16) & 0xFF));  // Extract the RR byte
	outColor += " ";
	outColor += std::to_string(((colorPart >> 8) & 0xFF));   // Extract the GG byte
	outColor += " ";
	outColor += std::to_string(((colorPart) & 0xFF));        // Extract the BB byte
	outColor += " ";
	return outColor;
}

void readFile(Puzzle* puzzle, string filePath)
{
	string line;
	string element;
	ifstream inputFile(filePath);
	int w, h, c;
	string b;
	int i, j;
	c = 0;
	b = DEFAULTBGCOLOR;

	if (inputFile.is_open())
	{
		getline(inputFile, line);
		istringstream dim(line);
		getline(dim, element, ',');
		w = stoi(element);
		getline(dim, element, ',');
		h = stoi(element);

		getline(inputFile, line);
		if (line != "") 
		{
			c = count(line.begin(), line.end(), ',') + 1;
		}

		puzzle = new Puzzle(w, h, c);
		puzzle->bgColor = DEFAULTBGCOLOR;

		if (c > 0)
		{
			istringstream bgColorChecker(line);
			if (count(line.begin(), line.end(), '|') > 0)
			{
				getline(bgColorChecker, element, '|');
				puzzle->bgColor = colorHexToRgb(element);
				getline(bgColorChecker, line, '|');
			}
			istringstream colorDim(line);
			int i = 0;
			while (getline(colorDim, element, ','))
			{
				puzzle->colors[i] = colorHexToRgb(element);
			}
			getline(inputFile, line);
		}

		i = 0;
		while (getline(inputFile, line) && i < puzzle->_width)
		{
			puzzle->tomographyWidth->sizes[i] = count(line.begin(), line.end(), ',') + 1;
			puzzle->tomographyWidth->tomography[i] = new Tomograph*[puzzle->tomographyWidth->sizes[i]];
			istringstream dim(line);
			j = 0;
			while (getline(dim, element, ','))
			{
				if (element != "0")
				{
					puzzle->tomographyWidth->tomography[i][j] = new Tomograph(stoi(element));
				}
				else
				{
					puzzle->tomographyWidth->tomography[i][j] = nullptr;
					puzzle->tomographyWidth->sizes[i] = 0;
				}
				j++;
			}
			i++;
		}

		i = 0;
		while (getline(inputFile, line) && i < puzzle->_height)
		{
			puzzle->tomographyHeight->sizes[i] = count(line.begin(), line.end(), ',') + 1;
			puzzle->tomographyHeight->tomography[i] = new Tomograph*[puzzle->tomographyHeight->sizes[i]];
			istringstream dim(line);
			j = 0;
			while (getline(dim, element, ','))
			{
				if (element != "0")
				{
					puzzle->tomographyHeight->tomography[i][j] = new Tomograph(stoi(element));
				}
				else
				{
					puzzle->tomographyHeight->tomography[i][j] = nullptr;
					puzzle->tomographyHeight->sizes[i] = 0;
				}
				j++;
			}
			i++;
		}

		inputFile.close();
	}
	else
	{
		std::cout << "Cannot open file";
	}
}
void outputPicture(Puzzle* puzzle, string filePath)
{
	int i, j;
	ofstream outputFile(filePath);
	if (outputFile.is_open())
	{
		outputFile << "P3\n" << puzzle->_width << " " << puzzle->_height << "\n255" << endl;
		j = 0;
		while (j < puzzle->_height)
		{
			i = 0;
			while (i < puzzle->_width)
			{

				if (puzzle->theGrid[i][j] > 1)
				{
					outputFile << puzzle->colors[puzzle->theGrid[i][j] - 2];
				}
				else
				{
					outputFile << puzzle->bgColor;
				}
				i++;
			}
			outputFile << endl;

			j++;
		}

		outputFile.close();
	}
	else
	{
		std::cout << "Cannot open file";
	}
}
/*
void readFile(Tomography* tomographyWidth, Tomography* tomographyHeight, string filePath)
{
	string line;
	string element;
	ifstream inputFile (filePath);
	int i,j;


	if (inputFile.is_open())
	{
		getline(inputFile, line);
		istringstream dim(line);
		getline(dim, element, ',');
		_width = stoi(element);
		getline(dim, element, ',');
		_height = stoi(element);
		tomographyWidth->tomography = new Tomograph**[_width];
		tomographyWidth->sizes = new int[_width];
		tomographyHeight->tomography = new Tomograph**[_height];
		tomographyHeight->sizes = new int[_height];

		getline(inputFile, line);
		if (line != "")  //Get colors in hex, convert to RGB
		{	
			istringstream bgColorChecker(line);
			if (count(line.begin(), line.end(), '|') > 0)
			{
				getline(bgColorChecker, element, '|');
				bgColor = colorHexToRgb(element);
				getline(bgColorChecker, line, '|');
			}
			else
			{
				bgColor = "255 255 255 ";
			}
			colors = new string[count(line.begin(), line.end(), ',') + 1];
			istringstream colorDim(line);
			int i = 0;
			while (getline(colorDim, element, ','))
			{
				colors[i] = colorHexToRgb(element);
			}
			getline(inputFile, line);
		}
		else
		{
			colors = new string[1];
			colors[0] = "0 0 0 ";
			bgColor = "255 255 255 ";
		}
		cout << colors[0];
		cout << bgColor;

		i = 0;
		while (getline(inputFile, line) && i < _width)
		{
			tomographyWidth->sizes[i] = count(line.begin(), line.end(), ',') + 1;
			tomographyWidth->tomography[i] = new Tomograph*[tomographyWidth->sizes[i]];
			istringstream dim(line);
			j = 0;
			while (getline(dim, element, ','))
			{
				if (element != "0")
				{
					tomographyWidth->tomography[i][j] = new Tomograph(stoi(element));
				}
				else
				{
					tomographyWidth->tomography[i][j] = nullptr;
					tomographyWidth->sizes[i] = 0;
				}
				j++;
			}
			i++;
		}

		i = 0;
		while (getline(inputFile, line) && i < _height)
		{
			tomographyHeight->sizes[i] = count(line.begin(), line.end(), ',') + 1;
			tomographyHeight->tomography[i] = new Tomograph*[tomographyHeight->sizes[i]];
			istringstream dim(line);
			j = 0;
			while (getline(dim, element, ','))
			{
				if (element != "0")
				{
					tomographyHeight->tomography[i][j] = new Tomograph(stoi(element));
				}
				else
				{
					tomographyHeight->tomography[i][j] = nullptr;
					tomographyHeight->sizes[i] = 0;
				}
				j++;
			}
			i++;
		}

		inputFile.close();
	}
	else
	{
		std::cout << "Cannot open file";
	}
}


void printTheGrid(short** theGrid)
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

bool bruteForceValidity(short** theGrid, Tomography* tomographyWidth, Tomography* tomographyHeight, int i, int j, bool& tooLong, time_t& startTime)
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
								printTheGrid(theGrid);
								if (time(0) - startTime > 10)
								{
									tooLong = true;
									startTime = time(0);
								}
							}
						}
						else if (time(0) - startTime > 2)
						{
							printTheGrid(theGrid);
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
					printTheGrid(theGrid);
					if (time(0) - startTime > 10)
					{
						tooLong = true;
						startTime = time(0);
					}
				}
				else if (time(0) - startTime > 2)
				{
					printTheGrid(theGrid);
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


void bruteForce(short** theGrid, Tomography* tomographyWidth, Tomography* tomographyHeight)
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
					theGrid[i][j] = 2;
				}
				valid = true;
				if (needBacktrack && theGrid[i][j] == 2)
				{
					theGrid[i][j] = 1;
					needBacktrack = false;
				}
				if (DEBUG)
				{
					printTheGrid(theGrid);
					//std::system("pause");
					//
				}
				
				valid = bruteForceValidity(theGrid, tomographyWidth, tomographyHeight, i, j, tooLong, startTime);

				if (!valid)
				{
					if (theGrid[i][j] == 2)
					{
						theGrid[i][j] = 1;
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
void outputPicture(short** theGrid, string filePath)
{
	int i, j;
	ofstream outputFile(filePath);
	if (outputFile.is_open())
	{
		outputFile << "P3\n" << _width << " " << _height <<"\n255"<<endl;
		j = 0;
		while (j < _height)
		{
			i = 0;
			while (i < _width)
			{
			
				if (theGrid[i][j] > 1)
				{
					outputFile << colors[theGrid[i][j] - 2];
				}
				else
				{
					outputFile << bgColor;
				}
				i++;
			}
			outputFile << endl;

			j++;
		}

		outputFile.close();
	}
	else
	{
		std::cout << "Cannot open file";
	}
}*/

int main()
{
	Puzzle* puzzle = nullptr;
	//Tomography* tomographyWidth = nullptr;
	//Tomography* tomographyHeight = nullptr;
	//tomographyWidth = new Tomography();
	//tomographyHeight = new Tomography();

	string inputFiles[] = {/*"C:\\Users\\holtza\\Documents\\nono\\5x5rune.txt", 
		"C:\\Users\\holtza\\Documents\\nono\\10x10tree.txt",
		"C:\\Users\\holtza\\Documents\\nono\\12x12bee.txt",
		"C:\\Users\\holtza\\Documents\\nono\\15x15trivial.txt",
		"C:\\Users\\holtza\\Documents\\nono\\15x15turtle.txt",
		//"C:\\Users\\holtza\\Documents\\nono\\15x20cock.txt",
		"C:\\Users\\holtza\\Documents\\nono\\20x15goldfish.txt",*/
		"C:\\Users\\holtza\\Documents\\nono\\20x20peacock.txt",//
		//"C:\\Users\\holtza\\Documents\\nono\\25x25lion.txt",
		//"C:\\Users\\holtza\\Documents\\nono\\80x80MichaelJackson.txt",
		//"C:\\Users\\holtza\\Documents\\nono\\C8x8Mushroom.txt" 
	};
	//std::system("pause");
	
	for (int i = 0; i < sizeof(inputFiles) / sizeof(inputFiles[0]); i++)
	{

		readFile(puzzle, inputFiles[0]);
		puzzle->bruteForce();
		outputPicture(puzzle, inputFiles[i].substr(0, inputFiles[i].size() - 3) + "ppm");
		cout << inputFiles[i] << endl;
		delete puzzle;
		//readFile(tomographyWidth, tomographyHeight, inputFiles[i]);

		/*
		short** theGrid;
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

		bruteForce(theGrid, tomographyWidth, tomographyHeight);
		outputPicture(theGrid, inputFiles[i].substr(0, inputFiles[i].size() - 3) +  "ppm");

		cout << inputFiles[i] << endl;//*/
	}

	

	std::system("pause");
	return 0;
}