#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <ctime>
#include "Puzzle.h"
#include "Globals.h"

using namespace std;

string colorHexToRgb(string hexColor);
void readFile(Puzzle*& puzzle, string filePath);
void outputPicture(Puzzle* puzzle, string filePath);

int main()
{
	Puzzle* puzzle = nullptr;

	string inputFiles[] = {//"80x80MichaelJackson.txt",  //Warning!!!
		//*
		//"5x5rune.txt", 
		"10x10tree.txt",
		"12x12bee.txt",
		"15x15trivial.txt",
		"15x15turtle.txt",
		//"15x20cock.txt",
		"20x15goldfish.txt",
		//*/
		//"20x20peacock.txt",
		//"25x25lion.txt",
		"C5x5Target.txt",
		"C8x8Mushroom.txt", 
		/*"C19x13BrazilFlag.txt",
		"C16x25Sakura.txt",
		"C18x25Match.txt",
		"C20x20Ladybug.txt",
		"C20x20Swan.txt"*/
	};
	//std::system("pause");
	
	for (int i = 0; i < sizeof(inputFiles) / sizeof(inputFiles[0]); i++)
	{
		inputFiles[i] = NONOPATH + inputFiles[i];
		readFile(puzzle, inputFiles[i]);
		if (DEBUG)
		{
			puzzle->tomographyWidth->print();
			puzzle->tomographyHeight->print();
		}
		//puzzle->bruteForce();
		puzzle->greedy();

		outputPicture(puzzle, inputFiles[i].substr(0, inputFiles[i].size() - 3) + "ppm");
		cout << inputFiles[i] << endl;
		delete puzzle;
	}

	

	std::system("pause");
	return 0;
}

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
void readFile(Puzzle*& puzzle, string filePath)
{
	string line;
	string element;
	string color;
	string value;
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
		if (c > 0)
		{
			puzzle = new Puzzle(w, h, c);
		}
		else
		{
			puzzle = new Puzzle(w, h, 1);
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
				i++;
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
					if (c > 0)
					{
						istringstream colorDim(element);
						getline(colorDim, value, '|');
						getline(colorDim, color, '|');
						puzzle->tomographyWidth->tomography[i][j] = new Tomograph(stoi(value), (short)stoi(color));
					}
					else
					{
						puzzle->tomographyWidth->tomography[i][j] = new Tomograph(stoi(element), 1);
					}
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
					if (c > 0)
					{
						istringstream colorDim(element);
						getline(colorDim, value, '|');
						getline(colorDim, color, '|');
						puzzle->tomographyHeight->tomography[i][j] = new Tomograph(stoi(value), (short)stoi(color));
					}
					else
					{
						puzzle->tomographyHeight->tomography[i][j] = new Tomograph(stoi(element), 1);
					}

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