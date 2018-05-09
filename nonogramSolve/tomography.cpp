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

Tomography::Tomography(const Tomography& obj)
{
	dimensionSize = obj.dimensionSize;
	sizes = new int[dimensionSize];
	for (int i = 0; i < dimensionSize; i++)
	{
		sizes[i] = obj.sizes[i];
	}

	tomography = new Tomograph**[dimensionSize];
	for (int i = 0; i < dimensionSize; i++)
	{
		tomography[i] = new Tomograph*[sizes[i]];
		for (int j = 0; j < sizes[i]; j++)
		{
			tomography[i][j] = new Tomograph(*obj.tomography[i][j]);
		}
	}
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