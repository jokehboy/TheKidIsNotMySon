/*
=================
cAsteroid.h
- Header file for class definition - SPECIFICATION
- Header file for the Rocket class which is a child of cSprite class
=================
*/
#ifndef _CASTEROID_H
#define _CASTEROID_H
#include "cSprite.h"

class cFall : public cSprite
{
private:
	int spriteVelocity;

public:
	cFall();
	void update(double deltaTime);		// Rocket update method
	void setSpriteVel(int spriteVel);   // Sets the velocity for the asteroid
	int getSpriteVel();				 // Gets the asteroid velocity
};
#endif