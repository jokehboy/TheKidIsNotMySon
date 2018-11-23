/*
==================================================================================
cGame.cpp
==================================================================================
*/
#include "cGame.h"


Mix_Music *music = Mix_LoadMUS("Sounds/BiliJen.mp3");

cGame* cGame::pInstance = NULL;
static cTextureMgr* theTextureMgr = cTextureMgr::getInstance();

//Smooth movement variables
//The movement is based upon framerate, I will code both an average reading of framerate into the game
//and a fixed framerate (just a made up variable) into my game and see which one works best.
//quick maffs n that
//Just realised the window is created with vsync so this was a big waste of time
#define FRAME_VALUES 10


float spriteDirX = 0.0f;










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

	this->m_lastTime = high_resolution_clock::now();

	theTextureMgr->setRenderer(theRenderer);
	theTextureMgr->addTexture("theBackground", "Images/Backdrop.png");
	
	spriteBkgd.setSpritePos({ 0, 0 });
	spriteBkgd.setTexture(theTextureMgr->getTexture("theBackground"));
	spriteBkgd.setSpriteDimensions(theTextureMgr->getTexture("theBackground")->getTWidth(), theTextureMgr->getTexture("theBackground")->getTHeight());

	
	theTextureMgr->addTexture("Spotlight", "Images/Spotlight.png");
	spriteSpotlight.setSpritePos({ 0,0 });
	spriteSpotlight.setTexture(theTextureMgr->getTexture("Spotlight"));
	spriteSpotlight.setSpriteDimensions(theTextureMgr->getTexture("Spotlight")->getTWidth(), theTextureMgr->getTexture("Spotlight")->getTHeight());

	theTextureMgr->addTexture("Player", "Images/Michael/MoonwalkSheet.png");
	playerSprite.setSpritePos({ 0,0 });
	playerSprite.setTexture(theTextureMgr->getTexture("Player"));
	
	
	


	theTextureMgr->addTexture("theRocket", "Images\\Coin.png");
	rocketSprite.setSpritePos({ 500, 350 });
	rocketSprite.setTexture(theTextureMgr->getTexture("theRocket"));
	spriteBkgd.setSpriteDimensions(theTextureMgr->getTexture("theRocket")->getTWidth(), theTextureMgr->getTexture("theRocket")->getTHeight());
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
	rocketSprite.render(theRenderer, &rocketSprite.getSpriteDimensions(), &rocketSprite.getSpritePos(), rocketSprite.getRocketRotation(), &rocketSprite.getSpriteCentre(), rocketSprite.getSpriteScale());
	spriteSpotlight.render(theRenderer, NULL, NULL, spriteSpotlight.getSpriteScale());
	playerSprite.animateSprite(0, 7, 29, 58);
	playerSprite.render(theRenderer, &playerSprite.getSpriteDimensions() ,NULL,playerSprite.getSpriteScale());
	
	
	
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
	if (Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 1024) < 0)
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
	
	rocketSprite.update(deltaTime);
}

int xVel, yVel;




bool cGame::getInput(bool theLoop)
{
	SDL_Rect xPos, yPos = rocketSprite.getSpritePos();

	SDL_Rect newPos = rocketSprite.getSpritePos();
	FPoint currentScale = rocketSprite.getSpriteScale();
	FPoint newScale;
	
	rocketSprite.setRocketVelocity(50);
	
	SDL_Event event;


	
	


	while (SDL_PollEvent(&event))
	{
		if (event.type == SDL_QUIT)
		{
			theLoop = false;
		}
		if (event.type == SDL_KEYDOWN )
		{
				
				
			if (event.key.keysym.sym == SDLK_ESCAPE)
			{

				theLoop = false;
			}
			if (event.key.keysym.sym == SDLK_a)
			{
				newPos.x -= 5;
				rocketSprite.setSpritePos(newPos);
				cout << "a pressed and registered" << endl;
			}
			if (event.key.keysym.sym == SDLK_d)
			{
				xVel += rocketSprite.getRocketVelocity();
				cout << "d pressed and registered" << endl;
			}
			if (event.key.keysym.sym == SDLK_w)
			{
				newPos.y -= 5;
				rocketSprite.setSpritePos(newPos);
				cout << "w pressed and registered" << endl;
			}
			if (event.key.keysym.sym == SDLK_s)
			{
				newPos.y += 5;
				rocketSprite.setSpritePos(newPos);
				cout << "s pressed and registered" << endl;
			}
			if (event.key.keysym.sym == SDLK_KP_4)
			{
				rocketSprite.setRocketRotation(rocketSprite.getRocketRotation() - 5);
				cout << "keypad 4 pressed and registered" << endl;
			}
			if (event.key.keysym.sym == SDLK_KP_6)
			{
				rocketSprite.setRocketRotation(rocketSprite.getRocketRotation() + 5);
				cout << "keypad 6 pressed and registered" << endl;
			}
			if (event.key.keysym.sym == SDLK_KP_8)
			{
				newScale.X = currentScale.X + 1;
				newScale.Y = currentScale.Y + 1;
				rocketSprite.setSpriteScale(newScale);
				rocketSprite.scaleSprite();

				cout << "keypad 8 pressed and registered" << endl;
			}
			if (event.key.keysym.sym == SDLK_KP_5)
			{
				newScale.X = currentScale.X - 1;
				newScale.Y = currentScale.Y - 1;
				rocketSprite.setSpriteScale(newScale);
				rocketSprite.scaleSprite();


				cout << "keypad 5 pressed and registered" << endl;
			}
			//SDL_Renderer* theRenderer, SDL_Rect* theSourceRect, SDL_Rect* theDestRect, FPoint theScaling
			
;
				
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

