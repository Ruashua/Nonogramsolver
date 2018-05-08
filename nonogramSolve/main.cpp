#include "Globals.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <ctime>
#ifdef GETEXECUTIONTIME
#include <chrono>
#endif //GETEXECUTIONTIME
#include "Puzzle.h"


using namespace std;
#ifdef GETEXECUTIONTIME
using namespace std::chrono;
#endif //GETEXECUTIONTIME

string colorHexToRgb(string hexColor);
void readFile(Puzzle*& puzzle, string filePath);
void outputPicture(Puzzle* puzzle, string filePath);

int main()
{
	Puzzle* puzzle = nullptr;
	

	string inputFiles[] = {
		
		//"5x5rune.txt", 
		//"10x10tree.txt",
		//"12x12bee.txt",
		//"15x15trivial.txt",
		//"15x15turtle.txt",
		//"15x20cock.txt",
		//"19x19-9-dom.txt",
		"20x15goldfish.txt",
		/*//"20x20peacock.txt",
		"25x25lion.txt",
		"C5x5Target.txt",
		"C8x8Mushroom.txt", 
		"C19x13BrazilFlag.txt",
		"C16x25Sakura.txt",
		"C18x25Match.txt",
		"C20x20Ladybug.txt",
		"C20x20Swan.txt" //*/
		//"47x40Sierp(VeryHard).txt",  //Warning!!!
		//"80x80MichaelJackson.txt",  //Warning!!!
	};




	//std::system("pause");

#ifdef GETEXECUTIONTIME
	int numberOfAlgorithms;
	numberOfAlgorithms = 0;
#ifdef RUNBRUTEFORCE
	numberOfAlgorithms++;
#endif // RUNBRUTEFORCE
#ifdef RUNGREEDY
	numberOfAlgorithms++;
#endif // RUNGREEDY

	int algorithmCounter;
	microseconds microTime;
	unsigned long long** executionTime;
	executionTime = new unsigned long long*[sizeof(inputFiles) / sizeof(inputFiles[0])];
	for (int i = 0; i < sizeof(inputFiles) / sizeof(inputFiles[0]); i++)
	{
		executionTime[i] = new unsigned long long[numberOfAlgorithms];
	}
#endif //GETEXECUTIONTIME

	for (int i = 0; i < sizeof(inputFiles) / sizeof(inputFiles[0]); i++)
	{

#ifdef GETEXECUTIONTIME
		algorithmCounter = 0;
#endif //GETEXECUTIONTIME

		readFile(puzzle, NONOPATH + inputFiles[i]);
		//puzzle->transposePuzzle();
		puzzle->mirrorPuzzle(puzzle->tomographyWidth, puzzle->tomographyHeight);

#ifdef DEBUG
		puzzle->tomographyWidth->print();
		cout << endl;
		puzzle->tomographyHeight->print();
		system("pause");
#endif //DEBUG

#ifdef RUNBRUTEFORCE
		puzzle->zeroTheGrid();
#ifdef GETEXECUTIONTIME
		microTime = duration_cast< microseconds >(
			system_clock::now().time_since_epoch()
			);
		executionTime[i][algorithmCounter] = microTime.count();
#endif //GETEXECUTIONTIME

		puzzle->bruteForce();

#ifdef GETEXECUTIONTIME
		microTime = duration_cast< microseconds >(
			system_clock::now().time_since_epoch()
			);
		executionTime[i][algorithmCounter] = microTime.count() - executionTime[i][algorithmCounter];
		algorithmCounter++;
#endif //GETEXECUTIONTIME
#endif // RUNBRUTEFORCE

#ifdef RUNGREEDY
		puzzle->zeroTheGrid();
#ifdef GETEXECUTIONTIME
		microTime = duration_cast< microseconds >(
			system_clock::now().time_since_epoch()
			);
		executionTime[i][algorithmCounter] = microTime.count();
#endif //GETEXECUTIONTIME

		puzzle->greedy();

#ifdef GETEXECUTIONTIME
		microTime = duration_cast< microseconds >(
			system_clock::now().time_since_epoch()
			);
		executionTime[i][algorithmCounter] = microTime.count() - executionTime[i][algorithmCounter];
		algorithmCounter++;
#endif //GETEXECUTIONTIME
#endif // RUNGREEDY

#ifdef RUNLIKEHUMAN
		puzzle->zeroTheGrid();
#ifdef GETEXECUTIONTIME
		microTime = duration_cast< microseconds >(
			system_clock::now().time_since_epoch()
			);
		executionTime[i][algorithmCounter] = microTime.count();
#endif //GETEXECUTIONTIME

		puzzle->greedy();

#ifdef GETEXECUTIONTIME
		microTime = duration_cast< microseconds >(
			system_clock::now().time_since_epoch()
			);
		executionTime[i][algorithmCounter] = microTime.count() - executionTime[i][algorithmCounter];
		algorithmCounter++;
#endif //GETEXECUTIONTIME
#endif // RUNLIKEHUMAN

		outputPicture(puzzle, NONOPATH + inputFiles[i].substr(0, inputFiles[i].size() - 3) + "ppm");
		cout << NONOPATH + inputFiles[i] << endl;
		delete puzzle;
	}

#ifdef GETEXECUTIONTIME
	std::system("cls");
	for (int i = 0; i < sizeof(inputFiles) / sizeof(inputFiles[0]); i++)
	{
		cout << inputFiles[i] << ":   ";
		for (int j = 0; j < numberOfAlgorithms; j++)
		{
			cout << j << ": " << executionTime[i][j] << char(230)<< "s  ";
		}
		cout << endl;
	}

	for (int i = 0; i < sizeof(inputFiles) / sizeof(inputFiles[0]); i++)
	{
		delete[] executionTime[i];
	}
	delete executionTime;
#endif //GETEXECUTIONTIME


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