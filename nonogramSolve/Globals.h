#pragma once


#include <string>

using namespace std;

//ALGORITHMS
//#define RUNBRUTEFORCE
#define RUNGREEDY
//#define RUNLIKEHUMAN
#define BUILDHEATMAP

#define PARALLEL

#define PNGPROCESSING
//#define PRETTYPRINT
//#define DEBUG
//#define GETEXECUTIONTIME
#define DEFAULTBGCOLOR "255 255 255 " //const string DEFAULTBGCOLOR = "255 255 255 ";
//const string NONOPATH = "C:\\Users\\Ruashua\\Documents\\nono\\";  //Home
//const string HEATMAPPATH = "C:\\Users\\Ruashua\\Documents\\nono\\heatmaps\\";  //Home
//const string NONOPATH = "C:\\nono\\";  //Other
const string NONOPATH = "C:\\Users\\holtza\\Documents\\nono\\";  //Work
const string HEATMAPPATH = "C:\\Users\\holtza\\Documents\\nono\\heatmaps\\";  //work
const int PRINTTOOLONG = 180;
const int PRINTTOOLONGINTERVAL = 1;
extern bool RACENOTFINISHED;
const char PRINTCHARS[] = { char(32), '.', char(219), char(178), char(177), char(176),
'$', '@', 'B', '%', '&', 'W', '#', '*', 'o', 'a', 'h', 'k', 'b', 'd', 'p',
'q', 'm', 'Z', '0', 'Q', 'L', 'C', 'J', 'U', 'Y', 'X', 'c', 'v', 'u',
'n', 'x', 'r', 'j', 'f', 't', '/', '|', '(', '1', '}', ']', '\?',
'-', '+', '~', '<', 'i', '!', 'l', 'I', ';', ',', '\"', '^', '`', '\'' };

const string inputFiles[] = {

	//*//*
	"5x5rune.txt",
	"10x10tree.txt",
	"12x12bee.txt",
	"15x15trivial.txt",
	"15x15turtle.txt",
	"15x20cock.txt",
	"19x19-9-dom.txt",
	"20x15goldfish.txt",
	"20x20peacock.txt",
	"25x25lion.txt",
	"C5x5Target.txt",
	"C8x8Mushroom.txt",
	"C19x13BrazilFlag.txt",
	"C16x25Sakura.txt",
	"C18x25Match.txt",
	"C20x20Ladybug.txt",
	"C20x20Swan.txt", 
	"C32x27Charizard (4).txt",//*///*/
	//"47x40Sierp(VeryHard).txt",  //Warning!!!
	//"80x80MichaelJackson.txt",  //Warning!!!
};

/*main(n)
{
float r,i,R,I,b;
for(i=-1;i<1;i+=.06,puts(""))
for(r=-2;I=i,(R=r)<1;r += .03, putchar(n + 31))
for (n = 0; b = I*I, 26>n++&&R*R + b<4; I = 2 * R*I + i, R = R*R - b + r); 
}*/

#ifdef PNGPROCESSING
#include <png.h>
#endif // PNGPROCESSING