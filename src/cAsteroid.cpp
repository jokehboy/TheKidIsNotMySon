/*
=================
cAsteroid.cpp
- Header file for class definition - IMPLEMENTATION
=================
*/
#include "cAsteroid.h"

/*
=================================================================
Defualt Constructor
=================================================================
*/
cFall::cFall() : cSprite()
{
	this->spriteVelocity = 0;
}
/*
=================================================================
Update the sprite position
=================================================================
*/

void cFall::update(double deltaTime)
{

	this->setSpriteRotAngle((float)(this->getSpriteRotAngle() +(5.0f * deltaTime))); 
	if (this->getSpriteRotAngle() > 360)
	{
		this->setSpriteRotAngle(this->getSpriteRotAngle() - 360.0f);
	}

	SDL_Rect currentSpritePos = this->getSpritePos();
	currentSpritePos.x += (int)(this->getSpriteTranslation().x * deltaTime);
	currentSpritePos.y -= (int)(this->getSpriteTranslation().y * deltaTime);

	this->setSpritePos({ currentSpritePos.x, currentSpritePos.y });
	this->setBoundingRect(this->getSpritePos());
}
/*
=================================================================
  Sets the velocity for the Asteroid
=================================================================
*/
void cFall::setSpriteVel(int spriteVel)
{
	spriteVelocity = spriteVel;
}
/*
=================================================================
  Gets the Asteroid velocity
=================================================================
*/
int cFall::getSpriteVel()
{
	return spriteVelocity;
}
