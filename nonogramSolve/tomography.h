#pragma once

#include "Tomograph.h"
#include <iostream>

using namespace std;

class Tomography
{
	

public:
	int dimensionSize;
	Tomograph*** tomography;
	int* sizes;

	Tomography(int n);
	Tomography();
	~Tomography();

	void print();
};

