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
	Tomography(const Tomography& obj);
	Tomography();
	~Tomography();

	void print();
};

