#pragma once

#include <string>

using namespace std;

//ALGORITHMS
//#define RUNBRUTEFORCE
#define RUNGREEDY
//#define RUNLIKEHUMAN

#define PARALLEL

//#define PRETTYPRINT
//#define DEBUG
//#define GETEXECUTIONTIME
#define DEFAULTBGCOLOR "255 255 255 " //const string DEFAULTBGCOLOR = "255 255 255 ";
const string NONOPATH = "C:\\Users\\Ruashua\\Documents\\nono\\";  //Home
//const string NONOPATH = "C:\\Users\\holtza\\Documents\\nono\\";  //Work
const int PRINTTOOLONG = 3;
const int PRINTTOOLONGINTERVAL = 2;
extern bool RACENOTFINISHED;
const char PRINTCHARS[] = { char(32), '.', char(219), char(178), char(177), char(176),
'$', '@', 'B', '%', '8', '&', 'W', 'M', '#', '*', 'o', 'a', 'h', 'k', 'b', 'd', 'p',
'q', 'w', 'm', 'Z', 'O', '0', 'Q', 'L', 'C', 'J', 'U', 'Y', 'X', 'z', 'c', 'v', 'u',
'n', 'x', 'r', 'j', 'f', 't', '/', '\\', '|', '(', '1', '}', ']', '\?',
'-', '_', '+', '~', '<', 'i', '!', 'l', 'I', ';', ':', ',', '\"', '^', '`', '\'' };
/*main(n)
{
float r,i,R,I,b;
for(i=-1;i<1;i+=.06,puts(""))
for(r=-2;I=i,(R=r)<1;r += .03, putchar(n + 31))
for (n = 0; b = I*I, 26>n++&&R*R + b<4; I = 2 * R*I + i, R = R*R - b + r); 
}*/