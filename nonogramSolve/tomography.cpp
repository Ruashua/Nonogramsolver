#include "Tomography.h"

Tomography::Tomography(int n)
{
	dimensionSize = n;
	tomography = new Tomograph**[dimensionSize];
	for (int i = 0; i < dimensionSize; i++)
	{
		tomography[i] = nullptr;
	}
	sizes = new int[dimensionSize];
}

Tomography::Tomography()
{
	tomography = nullptr;
	sizes = nullptr;
	dimensionSize = 0;
}


Tomography::~Tomography()
{
	for (int i = 0; i < dimensionSize; i++)
	{
		if (sizes != nullptr)
		{
			for (int j = 0; j < sizes[i]; j++)
			{
				delete tomography[i][j];
			}
		}
		delete[] tomography[i];
	}
	delete[] tomography;

	delete[] sizes;
}

void Tomography::print()
{
	for (int i = 0; i < dimensionSize; i++)
	{
		for (int j = 0; j < sizes[i]; j++)
		{
			cout << tomography[i][j]->number << "|" << tomography[i][j]->color << ',';
		}
		cout << endl;
	}
}