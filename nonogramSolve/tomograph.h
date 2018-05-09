#pragma once
class Tomograph
{
public:
	int number;
	short color;
	bool finished;
	int startPosition;

	Tomograph(int n, short c);
	Tomograph(const Tomograph& obj);
	Tomograph();
	~Tomograph();
};

