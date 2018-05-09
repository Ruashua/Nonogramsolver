#include "Tomograph.h"

Tomograph::Tomograph(int n, short c)
{
	number = n;
	color = c;
	finished = false;
}
Tomograph::Tomograph(const Tomograph& obj)
{
	number = obj.number;
	color = obj.color;
	finished = obj.finished;
	startPosition = obj.startPosition;
}
Tomograph::Tomograph()
{
	number = 0;
	finished = false;
}


Tomograph::~Tomograph()
{
}
