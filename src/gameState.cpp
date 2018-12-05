#include "rocketGame.h"

bool loadFiles() 
{
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


}