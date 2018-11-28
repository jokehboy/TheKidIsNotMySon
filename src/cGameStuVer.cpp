/*
==================================================================================
cGame.cpp
==================================================================================
*/
#include "cGame.h"
#include <sstream>
#include <cstring>

Mix_Music *music = Mix_LoadMUS("Sounds/BiliJen.mp3");

cGame* cGame::pInstance = NULL;
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

	startTime = SDL_GetTicks();
	animRate = 7;
	animLength = 8;
	

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


	//Setting player sprite textures.
	theTextureMgr->addTexture("Player0", "Images/Michael/Moon0.png");
	playerSprite[0].setSpritePos({ 500,525 });
	playerSprite[0].setTexture(theTextureMgr->getTexture("Player0"));
	playerSprite[0].setSpriteDimensions(theTextureMgr->getTexture("Player0")->getTWidth(), theTextureMgr->getTexture("Player0")->getTHeight());

	theTextureMgr->addTexture("Player1", "Images/Michael/Moon1.png");
	playerSprite[1].setTexture(theTextureMgr->getTexture("Player1"));
	playerSprite[1].setSpriteDimensions(theTextureMgr->getTexture("Player1")->getTWidth(), theTextureMgr->getTexture("Player1")->getTHeight());

	theTextureMgr->addTexture("Player2", "Images/Michael/Moon2.png");
	playerSprite[2].setTexture(theTextureMgr->getTexture("Player2"));
	playerSprite[2].setSpriteDimensions(theTextureMgr->getTexture("Player2")->getTWidth(), theTextureMgr->getTexture("Player2")->getTHeight());

	theTextureMgr->addTexture("Player3", "Images/Michael/Moon3.png");
	playerSprite[3].setTexture(theTextureMgr->getTexture("Player3"));
	playerSprite[3].setSpriteDimensions(theTextureMgr->getTexture("Player3")->getTWidth(), theTextureMgr->getTexture("Player3")->getTHeight());

	theTextureMgr->addTexture("Player4", "Images/Michael/Moon4.png");
	playerSprite[4].setTexture(theTextureMgr->getTexture("Player4"));
	playerSprite[4].setSpriteDimensions(theTextureMgr->getTexture("Player4")->getTWidth(), theTextureMgr->getTexture("Player4")->getTHeight());

	theTextureMgr->addTexture("Player5", "Images/Michael/Moon5.png");
	playerSprite[5].setTexture(theTextureMgr->getTexture("Player5"));
	playerSprite[5].setSpriteDimensions(theTextureMgr->getTexture("Player5")->getTWidth(), theTextureMgr->getTexture("Player5")->getTHeight());

	theTextureMgr->addTexture("Player6", "Images/Michael/Moon6.png");
	playerSprite[6].setTexture(theTextureMgr->getTexture("Player6"));
	playerSprite[6].setSpriteDimensions(theTextureMgr->getTexture("Player6")->getTWidth(), theTextureMgr->getTexture("Player6")->getTHeight());

	theTextureMgr->addTexture("Player7", "Images/Michael/Moon7.png");
	playerSprite[7].setTexture(theTextureMgr->getTexture("Player7"));
	playerSprite[7].setSpriteDimensions(theTextureMgr->getTexture("Player7")->getTWidth(), theTextureMgr->getTexture("Player7")->getTHeight());

	theTextureMgr->addTexture("Player8", "Images/Michael/Nod0.png");
	playerSprite[8].setTexture(theTextureMgr->getTexture("Player8"));
	playerSprite[8].setSpriteDimensions(theTextureMgr->getTexture("Player8")->getTWidth(), theTextureMgr->getTexture("Player8")->getTHeight());

	theTextureMgr->addTexture("Player9", "Images/Michael/Nod1.png");
	playerSprite[9].setTexture(theTextureMgr->getTexture("Player9"));
	playerSprite[9].setSpriteDimensions(theTextureMgr->getTexture("Player9")->getTWidth(), theTextureMgr->getTexture("Player9")->getTHeight());

	theTextureMgr->addTexture("Player10", "Images/Michael/Nod2.png");
	playerSprite[10].setTexture(theTextureMgr->getTexture("Player10"));
	playerSprite[10].setSpriteDimensions(theTextureMgr->getTexture("Player10")->getTWidth(), theTextureMgr->getTexture("Player10")->getTHeight());

	theTextureMgr->addTexture("Player11", "Images/Michael/Nod3.png");
	playerSprite[11].setTexture(theTextureMgr->getTexture("Player11"));
	playerSprite[11].setSpriteDimensions(theTextureMgr->getTexture("Player11")->getTWidth(), theTextureMgr->getTexture("Player11")->getTHeight());


	playerScale.X = 2;
	playerScale.Y = 2;








	
	
	
	

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


			newPos.x += 3;
			playerSprite->setSpritePos(newPos);
			
			if (frameDrawn >= 7) { frameDrawn = 0; }
			
			cout << frameDrawn << endl;
		
		}
		if (keystate[SDL_SCANCODE_A])
		{
			
			
			
			flipType = SDL_FLIP_HORIZONTAL;
			frameDrawn = ((SDL_GetTicks() - startTime) * animRate / 1000) % animLength;


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

