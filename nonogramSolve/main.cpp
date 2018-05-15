#include "Globals.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <ctime>
#ifdef GETEXECUTIONTIME
#include <chrono>
#endif //GETEXECUTIONTIME
#ifdef PARALLEL
#include <omp.h>
#endif //PARALLEL
#include <Windows.h>
#include "Puzzle.h"

using namespace std;
#ifdef GETEXECUTIONTIME
using namespace std::chrono;
#endif //GETEXECUTIONTIME

string colorHexToRgb(string hexColor);
void readFile(Puzzle*& puzzle, string filePath);
void outputPicture(Puzzle* puzzle, string filePath);
void outputHeatmaps(Puzzle* puzzle, string filePath)
{
	int* heatWidth, heatHeight;
	string** heatMap;


}

int main()
{
	Puzzle* puzzle = nullptr;
	

	

	std::system("pause");

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
		puzzle->transOrMirrorForParallel(4);
		
#ifdef DEBUG
		puzzle->tomographyWidth->print();
		cout << endl;
		puzzle->tomographyHeight->print();
		std::system("pause");
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

		RACENOTFINISHED = true;
#ifdef PARALLEL
		
#pragma omp parallel num_threads(8)
		{
			
			bool finished = false;
			Puzzle* localPuzzle;
			unsigned long long localTimeStart;
			microseconds localMicroTime;

			localPuzzle = new Puzzle(*puzzle);
			localPuzzle->transOrMirrorForParallel(omp_get_thread_num());
			
#pragma omp barrier
#ifdef GETEXECUTIONTIME
			localMicroTime = duration_cast< microseconds >(
				system_clock::now().time_since_epoch()
				);
			localTimeStart = localMicroTime.count();
#endif //GETEXECUTIONTIME

			finished = localPuzzle->greedy();

#ifdef GETEXECUTIONTIME
			localMicroTime = duration_cast< microseconds >(
				system_clock::now().time_since_epoch()
				);
#endif //GETEXECUTIONTIME

			if (finished)
			{
#pragma omp critical
				{
#ifdef GETEXECUTIONTIME
					executionTime[i][algorithmCounter] = localMicroTime.count() - localTimeStart;
#endif //GETEXECUTIONTIME
					std::cout << "Thread " << omp_get_thread_num() << " was first at " << executionTime[i][algorithmCounter] << char(230) << "s" << endl;
					delete puzzle;
					puzzle = localPuzzle;
					puzzle->undoTransOrMirrorForParallel(omp_get_thread_num());
				}
			}
			else
			{
				delete localPuzzle;
			}
		}
#else
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
#endif //GETEXECUTIONTIME
#endif //PARALLEL

#ifdef GETEXECUTIONTIME
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

		puzzle->undoTransOrMirrorForParallel(4);

		std::system("cls");
		puzzle->printTheGrid();
		Sleep(3000);
		outputPicture(puzzle, NONOPATH + inputFiles[i].substr(0, inputFiles[i].size() - 3) + "ppm");
		cout << inputFiles[i] << endl;
		delete puzzle;
	}

#ifdef GETEXECUTIONTIME
	//std::system("cls");
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
					//puzzle->tomographyWidth->tomography[i][j] = nullptr;
					puzzle->tomographyWidth->tomography[i][j] = new Tomograph(0, 0);
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
					//puzzle->tomographyHeight->tomography[i][j] = nullptr; 
					puzzle->tomographyHeight->tomography[i][j] = new Tomograph(0, 0);
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