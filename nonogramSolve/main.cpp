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

string valueToRainbow(int value)
{
	if (value < 0 || value >= 1276)
	{
		cout << "help";
	}
	int r, g, b;
	if (value < 256)
	{
		r = 255;
		g = value;
		b = 0;
	}
	else if (value < 511)
	{
		r = 510 - value;
		g = 255;
		b = 0;
	}
	else if (value < 766)
	{
		r = 0;
		g = 255;
		b = value - 510;
	}
	else if (value < 1021)
	{
		r = 0;
		g = 1020 - value;
		b = 255;
	}
	else if (value < 1276)
	{
		r = value - 1020;
		g = 0;
		b = 255;
	}
	else
	{
		r = 255;
		g = 255;
		b = 255;
	}
	return std::to_string(r) + " " + std::to_string(g) + " " + std::to_string(b) + " ";
}

void outputPicture(string** picture, int w, int h, string filePath)
{
	int i, j;
	ofstream outputFile(filePath);
	if (outputFile.is_open())
	{
		outputFile << "P3\n" << w << " " << h << "\n255" << endl;
		j = 0;
		while (j < h)
		{
			i = 0;
			while (i < w)
			{

				
				outputFile << picture[i][j];
				
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

void findColdestHottest(double& coldest, double& hottest, double** heatMap, int w, int h)
{
	

	for (int i = 0; i < w; i++)
	{
		for (int j = 0; j < h; j++)
		{
			if (heatMap[i][j] > hottest)
			{
				hottest = heatMap[i][j];
			}
			if (heatMap[i][j] < coldest)
			{
				coldest = heatMap[i][j];
			}
		}
	}
}

void buildStringHeatMap(string**& printHeatMap, double** heatMap, int w, int h, double coldest, double hottest)
{
	for (int i = 0; i < w; i++)
	{
		for (int j = 0; j < h; j++)
		{
			printHeatMap[i][j] = valueToRainbow((int)(1275 - 1275 * (heatMap[i][j] - coldest) / (hottest - coldest)));
		}
	}
}

double logProt(unsigned long long value)
{
	if (value != 0)
	{
		return log(value);
	}
	else
	{
		return 0;
	}
}

void outputHeatmaps(Puzzle* puzzle, std::string inputFile)
{
	unsigned long long* heatWidth, * heatHeight;
	double*** heatMaps;
	double hottest, coldest;
	std::string*** printHeatMap;
	std::string** allHeatMap;

	unsigned long long totalTop, totalBottom, totalLeft, totalRight;

	bool topBetter;
	bool leftBetter;
	
	printHeatMap = new std::string**[11];
	heatMaps = new double**[11];
	for (int k = 0; k < 11; k++)
	{
		printHeatMap[k] = new std::string*[puzzle->_width];
		heatMaps[k] = new double*[puzzle->_width];
		for (int i = 0; i < puzzle->_width; i++)
		{
			heatMaps[k][i] = new double[puzzle->_height];
			printHeatMap[k][i] = new std::string[puzzle->_height];
		}
	}

	allHeatMap = new std::string*[6 + 3 * puzzle->_width];
	for (int i = 0; i < (6 + 3 * puzzle->_width); i++)
	{
		allHeatMap[i] = new std::string[8 + 4 * puzzle->_height];
		
	}
	
	heatWidth = puzzle->tomographyHeat(puzzle->tomographyWidth);
	heatHeight = puzzle->tomographyHeat(puzzle->tomographyHeight);
	/////////////////////////////////////////////////////////////////////////////////////////

	
	totalLeft = totalRight = 0;
	for (int i = 0; i < puzzle->_width; i++)
	{
		totalLeft += heatWidth[i] * (puzzle->_width - i) / puzzle->_width;
		totalRight += heatWidth[i] * (i + 1) / puzzle->_width;
	}
	if (totalLeft < totalRight)
	{
		leftBetter = true;
	}
	else
	{
		leftBetter = false;
	}

	totalTop = totalBottom = 0;
	for (int i = 0; i < puzzle->_height; i++)
	{
		totalTop += heatHeight[i] * (puzzle->_height - i) / puzzle->_height;
		totalBottom += heatHeight[i] * (i + 1) / puzzle->_height;
	}
	if (totalTop < totalBottom)
	{
		topBetter = true;
	}
	else
	{
		topBetter = false;
	}
	cout << "Top: " << totalTop << " Bottom: " << totalBottom << " Left: " << totalLeft << " Right: " << totalRight;
	cout << endl;

	hottest = 0;
	coldest = DBL_MAX;
	for (int i = 0; i < puzzle->_width; i++)
	{
		for (int j = 0; j < puzzle->_height; j++)
		{
			heatMaps[0][i][j] = logProt(heatWidth[i]);  //width
			heatMaps[1][i][j] = logProt(heatWidth[i] * (puzzle->_width - i) / puzzle->_width);  //left
			heatMaps[2][i][j] = logProt(heatWidth[i] * (i + 1) / puzzle->_width);  //right

			heatMaps[3][i][j] = logProt(heatHeight[j]);  //height
			heatMaps[4][i][j] = logProt(heatHeight[j] * (puzzle->_height - j) / puzzle->_height);  //top
			heatMaps[5][i][j] = logProt(heatHeight[j] * (j + 1) / puzzle->_height);  //bottom

			heatMaps[6][i][j] = logProt(heatWidth[i] * heatHeight[j]);  //all
			heatMaps[7][i][j] = logProt(heatWidth[i] * heatHeight[j] * (puzzle->_width - i) * (puzzle->_height - j)  / puzzle->_width / puzzle->_height); //top left
			heatMaps[8][i][j] = logProt(heatHeight[j] * heatWidth[i] * (puzzle->_height - j) * (i + 1) / puzzle->_height / puzzle->_width);  //top right
			heatMaps[9][i][j] = logProt(heatHeight[j] * heatWidth[i] * (j + 1) * (puzzle->_width - i) / puzzle->_height / puzzle->_width);  //bottom left
			heatMaps[10][i][j] = logProt(heatHeight[j] * heatWidth[i] * (j + 1) * (i + 1) / puzzle->_height / puzzle->_width);  //bottom right
		}
	}

	hottest = 0;
	coldest = DBL_MAX;
	findColdestHottest(coldest, hottest, heatMaps[0], puzzle->_width, puzzle->_height);
	findColdestHottest(coldest, hottest, heatMaps[1], puzzle->_width, puzzle->_height);
	findColdestHottest(coldest, hottest, heatMaps[2], puzzle->_width, puzzle->_height);
	findColdestHottest(coldest, hottest, heatMaps[3], puzzle->_width, puzzle->_height);
	findColdestHottest(coldest, hottest, heatMaps[4], puzzle->_width, puzzle->_height);
	findColdestHottest(coldest, hottest, heatMaps[5], puzzle->_width, puzzle->_height);
	buildStringHeatMap(printHeatMap[0], heatMaps[0], puzzle->_width, puzzle->_height, coldest, hottest);
	buildStringHeatMap(printHeatMap[1], heatMaps[1], puzzle->_width, puzzle->_height, coldest, hottest);
	buildStringHeatMap(printHeatMap[2], heatMaps[2], puzzle->_width, puzzle->_height, coldest, hottest);
	buildStringHeatMap(printHeatMap[3], heatMaps[3], puzzle->_width, puzzle->_height, coldest, hottest);
	buildStringHeatMap(printHeatMap[4], heatMaps[4], puzzle->_width, puzzle->_height, coldest, hottest);
	buildStringHeatMap(printHeatMap[5], heatMaps[5], puzzle->_width, puzzle->_height, coldest, hottest);
	
	hottest = 0;
	coldest = DBL_MAX;
	findColdestHottest(coldest, hottest, heatMaps[6], puzzle->_width, puzzle->_height);
	findColdestHottest(coldest, hottest, heatMaps[7], puzzle->_width, puzzle->_height);
	findColdestHottest(coldest, hottest, heatMaps[8], puzzle->_width, puzzle->_height);
	findColdestHottest(coldest, hottest, heatMaps[9], puzzle->_width, puzzle->_height);
	findColdestHottest(coldest, hottest, heatMaps[10], puzzle->_width, puzzle->_height);
	buildStringHeatMap(printHeatMap[6], heatMaps[6], puzzle->_width, puzzle->_height, coldest, hottest);
	buildStringHeatMap(printHeatMap[7], heatMaps[7], puzzle->_width, puzzle->_height, coldest, hottest);
	buildStringHeatMap(printHeatMap[8], heatMaps[8], puzzle->_width, puzzle->_height, coldest, hottest);
	buildStringHeatMap(printHeatMap[9], heatMaps[9], puzzle->_width, puzzle->_height, coldest, hottest);
	buildStringHeatMap(printHeatMap[10], heatMaps[10], puzzle->_width, puzzle->_height, coldest, hottest);

	/*
	outputPicture(printHeatMap[0], puzzle->_width, puzzle->_height, HEATMAPPATH + inputFile + "WidthHeat.ppm");
	outputPicture(printHeatMap[1], puzzle->_width, puzzle->_height, HEATMAPPATH + inputFile + "WidthHeat.WeightedLeft.ppm");
	outputPicture(printHeatMap[2], puzzle->_width, puzzle->_height, HEATMAPPATH + inputFile + "WidthHeat.WeightedRight.ppm");
	outputPicture(printHeatMap[3], puzzle->_width, puzzle->_height, HEATMAPPATH + inputFile + "HeightHeat.ppm");
	outputPicture(printHeatMap[4], puzzle->_width, puzzle->_height, HEATMAPPATH + inputFile + "HeightHeat.WeightedTop.ppm");
	outputPicture(printHeatMap[5], puzzle->_width, puzzle->_height, HEATMAPPATH + inputFile + "HeightHeat.WeightedBottom.ppm");
	outputPicture(printHeatMap[6], puzzle->_width, puzzle->_height, HEATMAPPATH + inputFile + "BothHeat.ppm");
	outputPicture(printHeatMap[7], puzzle->_width, puzzle->_height, HEATMAPPATH + inputFile + "BothHeat.WeightedTopLeft.ppm");
	outputPicture(printHeatMap[8], puzzle->_width, puzzle->_height, HEATMAPPATH + inputFile + "BothHeat.WeightedTopRight.ppm");
	outputPicture(printHeatMap[9], puzzle->_width, puzzle->_height, HEATMAPPATH + inputFile + "BothHeat.WeightedBottomLeft.ppm");
	outputPicture(printHeatMap[10], puzzle->_width, puzzle->_height, HEATMAPPATH + inputFile + "BothHeat.WeightedBottomRight.ppm");
	*/
	
	int col[3];
	int row[4];
	col[0] = 0;
	col[1] = 2 + puzzle->_width;
	col[2] = 4 + 2 * puzzle->_width;

	row[0] = 0;
	row[1] = 2 + puzzle->_height;
	row[2] = 4 + 2 * puzzle->_height;
	row[3] = 6 + 3 * puzzle->_height;

	for (int i = 0; i < (6 + 3 * puzzle->_width); i++)
	{
		for (int j = 0; j < (8 + 4 * puzzle->_height); j++)
		{
			if (i > col[0] + 1 && i < col[1] && j > row[0] + 1 && j < row[1])
			{
				allHeatMap[i][j] = printHeatMap[0][i - 2][j - 2];
			}
			else if (i > col[1] + 1 && i < col[2] && j > row[0] + 1 && j < row[1])
			{
				allHeatMap[i][j] = printHeatMap[1][i - puzzle->_width - 4][j - 2];
			}
			else if (i > col[2] + 1 && i < 6 + 3 * puzzle->_width && j > row[0] + 1 && j < row[1])
			{
				allHeatMap[i][j] = printHeatMap[2][i - 2 * puzzle->_width - 6][j - 2];
			}

			else if (i > col[0] + 1 && i < col[1] && j > row[1] + 1 && j < row[2])
			{
				allHeatMap[i][j] = printHeatMap[3][i - 2][j - puzzle->_height - 4];
			}
			else if (i > col[1] + 1 && i < col[2] && j > row[1] + 1 && j < row[2])
			{
				allHeatMap[i][j] = printHeatMap[4][i - puzzle->_width - 4][j - puzzle->_height - 4];
			}
			else if (i > col[2] + 1 && i < 6 + 3 * puzzle->_width && j > row[1] + 1 && j < row[2])
			{
				allHeatMap[i][j] = printHeatMap[5][i - 2 * puzzle->_width - 6][j - puzzle->_height - 4];
			}

			else if (i > col[0] + 1 && i < col[1] && j > row[2] + 1 && j < row[3])
			{
				allHeatMap[i][j] = printHeatMap[6][i - 2][j - 2 * puzzle->_height - 6];
			}
			else if (i > col[1] + 1 && i < col[2] && j > row[2] + 1 && j < row[3])
			{
				allHeatMap[i][j] = printHeatMap[7][i - puzzle->_width - 4][j - 2 * puzzle->_height - 6];
			}
			else if (i > col[2] + 1 && i < 6 + 3 * puzzle->_width && j > row[2] + 1 && j < row[3])
			{
				allHeatMap[i][j] = printHeatMap[8][i - 2 * puzzle->_width - 6][j - 2 * puzzle->_height - 6];
			}

			else if (i > col[1] + 1 && i < col[2] && j > row[3] + 1 && j < 8 + 4 * puzzle->_height)
			{
				allHeatMap[i][j] = printHeatMap[9][i - puzzle->_width - 4][j - 3 * puzzle->_height - 8];
			}
			else if (i > col[2] + 1 && i < 6 + 3 * puzzle->_width && j > row[3] + 1 && j < 8 + 4 * puzzle->_height)
			{
				allHeatMap[i][j] = printHeatMap[10][i - 2 * puzzle->_width - 6][j - 3 * puzzle->_height - 8];
			}

			else if ((i == col[0] && j == row[0]) || (i == col[0] && j == row[0] + 1))  //Width symbol
			{
				allHeatMap[i][j] = "0 0 0 ";
			}
			else if((i == col[1] && j == row[0]) || (i == col[1] && j == row[0] + 1)) //Width left symbol
			{
				allHeatMap[i][j] = "0 0 0 ";
			}
			else if ((i == col[1] + 1 && j == row[0]) || (i == col[1] + 1 && j == row[0] + 1)) //Width left symbol
			{
				allHeatMap[i][j] = "128 128 128 ";
			}
			else if ((i == col[2] + 1 && j == row[0]) || (i == col[2] + 1 && j == row[0] + 1)) //Width right symbol
			{
				allHeatMap[i][j] = "0 0 0 ";
			}
			else if ((i == col[2] && j == row[0]) || (i == col[2] && j == row[0] + 1)) //Width right symbol
			{
				allHeatMap[i][j] = "128 128 128 ";
			}


			else if ((i == col[0] && j == row[1]) || (i == col[0] + 1 && j == row[1]))  //height symbol
			{
				allHeatMap[i][j] = "0 0 0 ";
			}
			else if ((i == col[1] && j == row[1]) || (i == col[1] + 1 && j == row[1])) //height left symbol
			{
				allHeatMap[i][j] = "0 0 0 ";
			}
			else if ((i == col[1] && j == row[1] + 1) || (i == col[1] + 1 && j == row[1] + 1)) //height left symbol
			{
				allHeatMap[i][j] = "128 128 128 ";
			}
			else if ((i == col[2] && j == row[1] + 1) || (i == col[2] + 1 && j == row[1] + 1))  //height right symbol
			{
				allHeatMap[i][j] = "0 0 0 ";
			}
			else if ((i == col[2] && j == row[1]) || (i == col[2] + 1 && j == row[1]))  //height right symbol
			{
				allHeatMap[i][j] = "128 128 128 ";
			}

			else if ((i == col[0] && j == row[2]) || (i == col[0] && j == row[2] + 1) || (i == col[0] + 1 && j == row[2]) || (i == col[0] + 1 && j == row[2] + 1))  //both symbol
			{
				allHeatMap[i][j] = "0 0 0 ";
			}
			else if (i == col[1] && j == row[2]) //top left symbol
			{
				allHeatMap[i][j] = "0 0 0 ";
			}
			else if((i == col[1] + 1 && j == row[2]) || (i == col[1] && j == row[2] + 1) || (i == col[1] + 1 && j == row[2] + 1)) //top left symbol
			{
				allHeatMap[i][j] = "128 128 128 ";
			}
			else if (i == col[2] + 1 && j == row[2]) //top right symbol
			{
				allHeatMap[i][j] = "0 0 0 ";
			}
			else if ((i == col[2] && j == row[2]) || (i == col[2] && j == row[2] + 1) || (i == col[2] + 1 && j == row[2] + 1)) //top right symbol
			{
				allHeatMap[i][j] = "128 128 128 ";
			}

			else if (i == col[1] && j == row[3] + 1) //bottom left symbol
			{
				allHeatMap[i][j] = "0 0 0 ";
			}
			else if ((i == col[1] + 1 && j == row[3]) || (i == col[1] && j == row[3]) || (i == col[1] + 1 && j == row[3] + 1)) //bottom left symbol
			{
				allHeatMap[i][j] = "128 128 128 ";
			}
			else if (i == col[2] + 1 && j == row[3] + 1) //bottom right symbol
			{
				allHeatMap[i][j] = "0 0 0 ";
			}
			else if ((i == col[2] + 1 && j == row[3]) || (i == col[2] && j == row[3] + 1) || (i == col[2] && j == row[3])) //bottom right symbol
			{
				allHeatMap[i][j] = "128 128 128 ";
			}

			else if (i == 0 && j == 6 + 4 * puzzle->_height)
			{
				if (topBetter && leftBetter)
				{
					cout << "Top-left";
					allHeatMap[i][j] = "0 0 0 ";
				}
				else if (topBetter && !leftBetter && totalTop <= totalRight)
				{
					allHeatMap[i][j] = "64 64 64 ";
				}
				else if (!topBetter && leftBetter && totalLeft <= totalBottom)
				{
					allHeatMap[i][j] = "64 64 64 ";
				}
				else
				{
					allHeatMap[i][j] = "128 128 128 ";
				}
			}
			else if (i == 1 && j == 6 + 4 * puzzle->_height)
			{
				if (topBetter && !leftBetter)
				{
					allHeatMap[i][j] = "0 0 0 ";
				}
				else if (topBetter && leftBetter && totalTop < totalLeft)
				{
					allHeatMap[i][j] = "64 64 64 ";
				}
				else if (!topBetter && !leftBetter && totalRight < totalBottom)
				{
					allHeatMap[i][j] = "64 64 64 ";
				}
				else
				{
					allHeatMap[i][j] = "128 128 128 ";
				}
			}
			else if (i == 0 && j == 7 + 4 * puzzle->_height)
			{
				if (!topBetter && leftBetter)
				{
					allHeatMap[i][j] = "0 0 0 ";
				}
				else if (topBetter && leftBetter && totalLeft <= totalTop)
				{
					allHeatMap[i][j] = "64 64 64 ";
				}
				else if (!topBetter && !leftBetter && totalBottom <= totalRight)
				{
					allHeatMap[i][j] = "64 64 64 ";
				}
				else
				{
					allHeatMap[i][j] = "128 128 128 ";
				}
			}
			else if (i == 1 && j == 7 + 4 * puzzle->_height)
			{
				if (!topBetter && !leftBetter)
				{
					allHeatMap[i][j] = "0 0 0 ";
				}
				else if (!topBetter && leftBetter && totalBottom < totalLeft)
				{
					allHeatMap[i][j] = "64 64 64 ";
				}
				else if (topBetter && !leftBetter && totalRight < totalTop)
				{
					allHeatMap[i][j] = "64 64 64 ";
				}
				else
				{
					allHeatMap[i][j] = "128 128 128 ";
				}
			}

			else
			{
				allHeatMap[i][j] = "255 255 255 ";
			}
		}
	}

	outputPicture(allHeatMap, 6 + 3 * puzzle->_width, 8 + 4 * puzzle->_height, HEATMAPPATH + inputFile + "allHeat.ppm");
	

	delete[] heatWidth;
	delete[] heatHeight;
	
	for (int k = 0; k < 11; k++)
	{
		for (int i = 0; i < puzzle->_width; i++)
		{
			delete[] heatMaps[k][i];
			delete[] printHeatMap[k][i];
		}
		delete[] heatMaps[k];
		delete[] printHeatMap[k];
	}
	delete[] heatMaps;
	delete[] printHeatMap;

	
	for (int i = 0; i < 6 + 3 * puzzle->_width; i++)
	{
		delete[] allHeatMap[i];
	}
	delete[] allHeatMap;
}

int main()
{
	Puzzle* puzzle = nullptr;
	

	

	std::system("pause");

	int numberOfAlgorithms;
	numberOfAlgorithms = 0;
	int algorithmCounter;
#ifdef RUNBRUTEFORCE
	numberOfAlgorithms++;
#endif // RUNBRUTEFORCE
#ifdef RUNGREEDY
	numberOfAlgorithms++;
#endif // RUNGREEDY
#ifdef GETEXECUTIONTIME
	


	
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
#ifdef GETEXECUTIONTIME
			unsigned long long localTimeStart;
			microseconds localMicroTime;
#endif //GETEXECUTIONTIME
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
					std::cout << "Thread " << omp_get_thread_num() << " was first ";
#ifdef GETEXECUTIONTIME
					executionTime[i][algorithmCounter] = localMicroTime.count() - localTimeStart;
					cout << "at " << executionTime[i][algorithmCounter] << char(230) << "s";
#endif //GETEXECUTIONTIME
					cout << endl;
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

#ifdef BUILDHEATMAP
		outputHeatmaps(puzzle, inputFiles[i].substr(0, inputFiles[i].size() - 3));
#endif // BUILDHEATMAP

#ifdef PRETTYPRINT
		std::system("cls");
		puzzle->printTheGrid();
		Sleep(3000);
#endif // PRETTYPRINT

		

		if (numberOfAlgorithms > 0)
		{
			outputPicture(puzzle, NONOPATH + inputFiles[i].substr(0, inputFiles[i].size() - 3) + "ppm");
		}
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