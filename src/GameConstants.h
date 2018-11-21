#ifndef _GAMECONSTANTS_H
#define _GAMECONSTANTS_H

// Windows & SDL 
#include <stdlib.h>
#include <Windows.h>
#include <iostream>
#include <string>
#include <chrono>
#include <cstdlib>

using namespace std::chrono;

#include <SDL.h>
// Image Texture loading library
#include <SDL_image.h>
// Font loading library
#include <SDL_ttf.h>
// Sound loading library
#include <SDL_mixer.h>

// Maths functions
#include <math.h>
constexpr auto PI = 3.14159265;

// STL Container & Algorithms
#include <vector>
#include <map>
#include <algorithm>

using namespace std;

//Define the string to appear in the top left corner of the window
#define WINDOW_TITLE "Space Rockets - Using Transforms!"

// This header file contains all the constants & enumarated types for the game
typedef struct fpoint
{
	float  X;
	float  Y;
}FPoint;

inline SDL_Point operator+(SDL_Point const &a, SDL_Point const &b)
{
	return SDL_Point{ a.x + b.x , a.y + b.y };
}

#endif