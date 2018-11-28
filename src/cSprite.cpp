/*
=================
cSprite.cpp
- Header file for class definition - IMPLEMENTATION
=================
*/
#include "cSprite.h"
/*
=================
- Data constructor initializes the cSprite to the data passed to
- the constructor from the paramater sPosition.
- Is always called, thus ensures all sprite objects are in a consistent state.
=================
*/
cSprite::cSprite() 			// Default constructor
{

	this->spritePos_2D = { 0, 0, 0, 0 };
	this->spriteTexture = NULL;
	this->spriteCentre = {0, 0};
	this->spriteScale = { 1, 1 };
}

cSprite::cSprite(cTexture* theSpriteTexture) 			// Default constructor
{
	this->spriteTexture = theSpriteTexture;
	this->setSpriteDimensions(this->spriteTexture->getTWidth(), this->spriteTexture->getTHeight());
	this->spritePos_2D = { 0, 0, this->spriteTexture->getTWidth(), this->spriteTexture->getTHeight() };
	this->spriteCentre = { this->spritePos_2D.w / 2, this->spritePos_2D.h / 2};
	this->spriteScale = { 1, 1 };
}

/*
=================
- Destructor
=================
*/
cSprite::~cSprite()			// Destructor
{
}
/*
=================
- Return the sprites current position.
=================
*/

SDL_Rect cSprite::getSpritePos()  // Return the sprites current position
{
	return this->spritePos_2D;
}

/*
=================
- set the position of the sprite.
=================
*/

void cSprite::setSpritePos(SDL_Rect sPosition)  // set the position of the sprite
{
	this->spritePos_2D.x = sPosition.x;
	this->spritePos_2D.y = sPosition.y;
	this->spritePos_2D.w = sPosition.w;
	this->spritePos_2D.h = sPosition.h;
}

/*
=================
- Return the sprites current image.
=================
*/

cTexture* cSprite::getTexture()  // Return the sprites current image
{
	return this->spriteTexture;
}

/*
=================
- set the image of the sprite.
=================
*/

void cSprite::setTexture(cTexture* theSpriteTexture)  // set the image of the sprite
{
	this->spriteTexture = theSpriteTexture;
	this->setSpriteDimensions(spriteTexture->getTWidth(), spriteTexture->getTHeight());
	this->spritePos_2D.w = textureWidth;
	this->spritePos_2D.h = textureHeight;
	this->spriteCentre = { this->spritePos_2D.w / 2, this->spritePos_2D.h / 2 };
}





void cSprite::render(SDL_Renderer* theRenderer, SDL_Rect* theSourceRect, SDL_Rect* theDestRect, FPoint theScaling)
{
	this->spriteTexture->renderTexture(theRenderer, this->spriteTexture->getTexture(), theSourceRect, theDestRect, theScaling);
}

void cSprite::render(SDL_Renderer* theRenderer, SDL_Rect* theSourceRect, SDL_Rect* theDestRect, double rotAngle, SDL_Point* spriteCentre, FPoint theScaling,SDL_RendererFlip flip)
{
	this->spriteTexture->renderTexture(theRenderer, this->spriteTexture->getTexture(), theSourceRect, theDestRect, rotAngle, spriteCentre, theScaling, flip);
}



/*
=================
- Set the sprite dimensions.
=================
*/
void cSprite::setSpriteDimensions(int texWidth, int textHeight)
{
	this->textureWidth = texWidth;
	this->textureHeight = textHeight;
	this->spriteDimensions = { 0, 0, texWidth, textHeight };
}

/*
=================
- Get the sprite dimensions.
=================
*/
SDL_Rect cSprite::getSpriteDimensions()
{
	return this->spriteDimensions;
}
/*
=================
- Return the sprite centre.
=================
*/

SDL_Point cSprite::getSpriteCentre()  // Return the sprites current position
{
	return this->spriteCentre;
}

/*
=================
- set the the sprite centre.
=================
*/

void cSprite::setSpriteCentre(SDL_Point sCentre)  // set the position of the sprite
{
	this->spriteCentre.x = sCentre.x;
	this->spriteCentre.y = sCentre.y;
}
/*
=================
- Return the sprite scaling.
=================
*/

FPoint cSprite::getSpriteScale()  // Return the sprites current scaling
{
	return this->spriteScale;
}

/*
=================
- set the the sprite scale.
=================
*/

void cSprite::setSpriteScale(FPoint sScale)  // set the sprites current scaling
{
	this->spriteScale.X = sScale.X;
	this->spriteScale.Y = sScale.Y;
}
/*
=================
- Update sprite scale.
=================
*/

void cSprite::scaleSprite()  // set the sprites current scaling
{
	// Scale sprite
	this->spritePos_2D.w = (int)(this->spriteDimensions.w * this->spriteScale.X);
	this->spritePos_2D.h = (int)(this->spriteDimensions.h * this->spriteScale.Y);
	// Scale Sprite Centre for rotation.
	this->spriteCentre.x = this->spritePos_2D.w / 2;
	this->spriteCentre.y = this->spritePos_2D.h / 2;
}




//int curFrame = 0;
//int maxframe = 7;
//SDL_Surface *loadedImage;
//SDL_Surface *image;

//loadedImage = IMG_Load("Images / Michael / MoonwalkSheet.png");
//
//SDL_Rect frame;
//29 58