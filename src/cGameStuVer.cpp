/*
==================================================================================
cGame.cpp
==================================================================================
*/
#include "GameConstants.h"
#include "cTexture.h"
#include "cTextureMgr.h"
#include "cFont.h"
#include "cFontMgr.h"
#include "cButtonMgr.h"
#include "cButton.h"
#include "cSprite.h"
#include "cGame.h"
#include <sstream>
#include <cstring>
#include "cFontMgr.h"
#include "cButtonMgr.h"
#include "cButton.h"



cGame* cGame::pInstance = NULL;

static cFontMgr* theFontMgr = cFontMgr::getInstance();
static cButtonMgr* theButtonMgr = cButtonMgr::getInstance();
static cTextureMgr* theTextureMgr = cTextureMgr::getInstance();

//Smooth movement variables
//The movement is based upon framerate, I will code both an average reading of framerate into the game
//and a fixed framerate (just a made up variable) into my game and see which one works best.
//quick maffs n that
//Just realised the window is created with vsync so this was a big waste of time

const Uint8 *keystate;

int animRate;
int animLength;

bool buttonPressed = false;
SDL_Texture *playerTextures[];

int frameDrawn;
int startTime;
FPoint playerScale;
SDL_RendererFlip flipType = SDL_FLIP_NONE;




Mix_Music *music = Mix_LoadMUS("Sounds/BiliJen.mp3");
Mix_Music *oof = Mix_LoadMUS("Sounds/oof.mp3");

int playerPosX;







/*
=================================================================================
Constructor
=================================================================================
*/
cGame::cGame()
{

}
/*
=================================================================================
Singleton Design Pattern
=================================================================================
*/
cGame* cGame::getInstance()
{
	if (pInstance == NULL)
	{
		pInstance = new cGame();
	}
	return cGame::pInstance;
}


void cGame::initialise(SDL_Window* theSDLWND, SDL_Renderer* theRenderer)
{
	// Clear the buffer with a black background
	SDL_SetRenderDrawColor(theRenderer, 0, 0, 0, 255);
	SDL_RenderPresent(theRenderer);
	
	theTextureMgr->setRenderer(theRenderer);

	startTime = SDL_GetTicks();
	animRate = 7;
	animLength = 8;
	playerScale.X = 2;
	playerScale.Y = 2;


	this->m_lastTime = high_resolution_clock::now();
	theTextureMgr->setRenderer(theRenderer);

	textureName = { "Player0","Player1","Player2","Player3","Player4","Player5","Player6","Player7","Player8","Player9","Player10","Player11", "theBackground" , "Spotlight" };
	texturesToUse = {"Images/Michael/Moon0.png","Images/Michael/Moon1.png","Images/Michael/Moon2.png","Images/Michael/Moon3.png","Images/Michael/Moon4.png","Images/Michael/Moon5.png","Images/Michael/Moon6.png","Images/Michael/Moon7.png","Images/Michael/Nod0.png","Images/Michael/Nod1.png","Images/Michael/Nod2.png","Images/Michael/Nod3.png", "Images/Backdrop.png" , "Images/Spotlight.png" };
	
	for (unsigned int tCount = 0; tCount < textureName.size(); tCount++)
	{
		theTextureMgr->addTexture(textureName[tCount], texturesToUse[tCount]);
	}

	tempTexture = theTextureMgr->getTexture("theBackground");
	
	btnNameList = { "start_btn" , "quit_btn" , "retry_btn" };
	btnTexturesToUse = {""}

	

	
	


	





}











void cGame::run(SDL_Window* theSDLWND, SDL_Renderer* theRenderer)
{

	
	bool loop = true;

	

	if (loop){ playMusic();  }
	while (loop)
	{
	
		
		//We get the time that passed since the last frame
		double elapsedTime = this->getElapsedSeconds();

		loop = this->getInput(loop);
		this->update(elapsedTime);
		this->render(theSDLWND, theRenderer);
		
	}
	
}





void cGame::render(SDL_Window* theSDLWND, SDL_Renderer* theRenderer)
{
	


	SDL_RenderClear(theRenderer);
	spriteBkgd.render(theRenderer, NULL, NULL, spriteBkgd.getSpriteScale());
	playerSprite[frameDrawn].render(theRenderer, &playerSprite[frameDrawn].getSpriteDimensions(), &playerSprite->getSpritePos(), NULL, &playerSprite[frameDrawn].getSpriteCentre(), playerSprite[frameDrawn].getSpriteScale(), flipType);
	spriteSpotlight.render(theRenderer, NULL, NULL, spriteSpotlight.getSpriteScale());
	SDL_RenderPresent(theRenderer);
}

void cGame::render(SDL_Window* theSDLWND, SDL_Renderer* theRenderer, double rotAngle, SDL_Point* spriteCentre)
{

	SDL_RenderPresent(theRenderer);
}

void cGame::update()
{
	

}

void cGame::playMusic()
{
	

	if (Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 1024) == -1)
	{
		cout << "Mixer init error: " << Mix_GetError() << endl;
	}
	
	int flags = MIX_INIT_MP3;
	int result = 0;

	
	if (SDL_INIT_AUDIO < 1)
	{
		cout << "Failed to init audio" << Mix_GetError() << endl;
	}
	if (flags != (result = Mix_Init(flags)))
	{
		cout<< "Couldn't init mixer ERROR " << Mix_GetError() << endl;
	}
	
	
	Mix_PlayMusic(music, -1);
	cout << "Mixer INIT " << endl;


}


void cGame::update(double deltaTime)
{
	if (buttonPressed == false)
	{
		
		frameDrawn = 8;
		frameDrawn = ((SDL_GetTicks() - startTime) * 2 / 1000) % 4 + 8;

		cout << frameDrawn << " ";


		if (frameDrawn > 12 || frameDrawn < 8 ) { frameDrawn = 8; }
		

	}
	
	
	rocketSprite.update(deltaTime);
	
}

int xVel, yVel;




bool cGame::getInput(bool theLoop)
{
	playerScale.X, playerScale.Y = 2;
	playerSprite->setSpriteScale(playerScale);
	playerSprite->scaleSprite();

	SDL_Rect newPos = playerSprite->getSpritePos();
	
	
	
	SDL_Event event;


	
	
	
	
	while (SDL_PollEvent(&event))
	{
		if (event.type == SDL_QUIT)
		{
			theLoop = false;
		}

		keystate = SDL_GetKeyboardState(NULL);

		if (keystate[SDL_SCANCODE_D])
		{
			
			flipType = SDL_FLIP_NONE;
			frameDrawn = ((SDL_GetTicks() - startTime) * animRate / 1000) % animLength;

			playerPosX += 3;
			newPos.x += 3;
			playerSprite->setSpritePos(newPos);
			
			if (frameDrawn >= 7) { frameDrawn = 0; }
			
			cout << frameDrawn << endl;
		
		}


		if (keystate[SDL_SCANCODE_A])
		{
			
			flipType = SDL_FLIP_HORIZONTAL;
			frameDrawn = ((SDL_GetTicks() - startTime) * animRate / 1000) % animLength;

			playerPosX -= 3;
			newPos.x -= 3;
			playerSprite->setSpritePos(newPos);
			
			if (frameDrawn >= 7) { frameDrawn = 0; }
			
			cout << frameDrawn << endl;
			
		}

		
		if (event.type == SDL_KEYUP)
		{
			if (event.key.keysym.sym == SDLK_a)
			{
				buttonPressed = false;
			}

			if (event.key.keysym.sym == SDLK_d)
			{
				buttonPressed = false;
			}
			
		}


		if (event.type == SDL_KEYDOWN)
		{

			if (event.key.keysym.sym == SDLK_ESCAPE)
			{
				theLoop = false;
			}

			if (event.key.keysym.sym == SDLK_a)
			{
				buttonPressed = true;
			}

			if (event.key.keysym.sym == SDLK_d)
			{
				buttonPressed = true;
			}

		}

	
		if ((newPos.x > 1024))
		{
			newPos.x = -22;
		}
		if ((newPos.x < -23))
		{
			newPos.x = 1023;
		}


	}

	 

	return theLoop;
}


void cGame::move(Uint32 deltaTime)
{
	
	
}

double cGame::getElapsedSeconds()
{
	this->m_CurrentTime = high_resolution_clock::now();
	this->deltaTime = (this->m_CurrentTime - this->m_lastTime);
	this->m_lastTime = this->m_CurrentTime;
	return deltaTime.count();
}



void cGame::cleanUp(SDL_Window* theSDLWND)
{
	// Delete our OpengL context
	SDL_GL_DeleteContext(theSDLWND);

	// Destroy the window
	SDL_DestroyWindow(theSDLWND);

	// Quit IMG system
	IMG_Quit();

	// Shutdown SDL 2
	SDL_Quit();
}

