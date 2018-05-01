#include "Tomograph.h"

Tomograph::Tomograph(int n, short c)
{
	number = n;
	color = c;
	finished = false;
}

Tomograph::Tomograph()
{
	number = 0;
	finished = false;
}


Tomograph::~Tomograph()
{
}
