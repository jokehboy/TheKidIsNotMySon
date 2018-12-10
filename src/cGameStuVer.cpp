/*
==================================================================================
cGame.cpp
==================================================================================
*/

#include "cGame.h"
#include "cFontMgr.h"
#include "cButtonMgr.h"


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



bool buttonPressed = false;
SDL_Texture *playerTextures[];



int animRate;//Animation rate and lenght are used to set the speed of frame change.
int animLength;
int frameDrawn;//Frame drawn is the current frame of animation, this is used to move between frames.
int startTime;
SDL_Point newPos; //Used to keep track of player position and move the player.
FPoint playerScale;//Used to scale the player sprites.
SDL_RendererFlip flipType = SDL_FLIP_NONE; //Used to flip the sprite on direction change.

SDL_Event event;	//Event created to handle differnt types of SDL events


//Music and sound effects created
Mix_Music *music = Mix_LoadMUS("Sounds/BiliJen.mp3");
Mix_Music *oof = Mix_LoadMUS("Sounds/oof.mp3");

 





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
	SDL_GetRendererOutputSize(theRenderer, &renderWidth, &renderHeight);
	SDL_SetRenderDrawColor(theRenderer, 0, 0, 0, 255);
	SDL_RenderPresent(theRenderer);
	this->m_lastTime = high_resolution_clock::now();
	theTextureMgr->setRenderer(theRenderer);
	

	mouseClicked = { 0,0 };

	//Setting up the walking animation rate and lenght (Later I just use numbers as all the player animations are in one array).
	startTime = SDL_GetTicks();
	animRate = 7;
	animLength = 8;

	//Player scale set so that Michael isnt tiny.
	playerScale.X = 2;
	playerScale.Y = 2;

	//Init font manager, if it doesnt init an error will apear.
	theFontMgr->initFontLib();


	fontList = { "Harlow","Walkway" };
	fontToUse = { "HARLOWSI.ttf" , "Walkway_UltraBold_0.ttf" };

	for (unsigned int i = 0; i < fontList.size(); i++)
	{
		if(i == 0){ theFontMgr->addFont(fontList[i], fontToUse[i], 48); }
		if(i==1){ theFontMgr->addFont(fontList[i], fontToUse[i], 24); }
	}

	gameText = { "Title" , "Avoid" , "Collect" , "Movement" , "Score" , "EndScreen" , "HighScore" };
	gameTextContent = { "That Kid is Not My Son" , "Avoid babies: Those kids aren't your son!" , "Collect roses from your fans!" , "Use the A and D keys to move" , "Score: " , "But she claims that she is the one... ", "High score: " };

	for (unsigned int  i = 0; i < gameText.size(); i++)
	{
		if (i == 0 || i == 4 || i == 6)
		{
			theTextureMgr->addTexture(gameText[i], theFontMgr->getFont("Harlow")->createTextTexture(theRenderer, gameTextContent[i], textType::solid, { 242,82,173,255 }, { 0, 0, 0, 0 }));
		}
		else
		{
			theTextureMgr->addTexture(gameText[i], theFontMgr->getFont("Walkway")->createTextTexture(theRenderer, gameTextContent[i], textType::solid, { 44, 203, 112, 255 }, { 0, 0, 0, 0 }));

		}

	}


	btnNameList = { "Start", "Exit" , "Replay" };
	btnTexturesToUse = { "Images/Play.png", "Images/Quit.png" , "Images/Retry.png" };
	btnPos = { {100,50} , {100,90} , {30,40} };

	for (unsigned int i = 0; i < btnNameList.size(); i++)
	{
		theTextureMgr->addTexture(btnNameList[i], btnTexturesToUse[i]);
	}
	for (unsigned int i = 0; i < btnNameList.size(); i++)
	{
		cButton * newBtn = new cButton();
		newBtn->setTexture(theTextureMgr->getTexture(btnNameList[i]));
		newBtn->setSpritePos(btnPos[i]);
		newBtn->setSpriteDimensions(theTextureMgr->getTexture(btnNameList[i])->getTWidth(), theTextureMgr->getTexture(btnNameList[i])->getTHeight());
		theButtonMgr->add(btnNameList[i], newBtn);
	}
	
	theGameState = gameState::menu;
	

	//Populate the vectors provided with names and file locations.
	textureName = { "Player0","Player1","Player2","Player3","Player4","Player5","Player6","Player7","Player8","Player9","Player10","Player11", "theBackground" , "Spotlight" };
	texturesToUse = {"Images/Michael/Moon0.png","Images/Michael/Moon1.png","Images/Michael/Moon2.png","Images/Michael/Moon3.png","Images/Michael/Moon4.png","Images/Michael/Moon5.png","Images/Michael/Moon6.png","Images/Michael/Moon7.png","Images/Michael/Nod0.png","Images/Michael/Nod1.png","Images/Michael/Nod2.png","Images/Michael/Nod3.png", "Images/Backdrop.png" , "Images/Spotlight.png" };
	//Add useable textures and apply a name to them using the image locations provided.
	for (unsigned int tCount = 0; tCount < textureName.size(); tCount++)
	{
		theTextureMgr->addTexture(textureName[tCount], texturesToUse[tCount]);
	}


	//Add the useable textures in populating the player sprite array for animation.
	for (int i = 0; i <= 11;i++)
	{
		playerSprite[i].setTexture(theTextureMgr->getTexture(textureName.at(i)));
	}
	
	//Set the first sprite frames position as the rest of the sprites inherit it.
	playerSprite[0].setSpritePos({ 500,525 });

	//Scale all the sprites
	playerScale.X, playerScale.Y = 1;
	playerSprite->setSpriteScale(playerScale);
	playerSprite->scaleSprite();

	//Create and position spotlight and backdrop
	spriteSpotlight.setSpritePos({ 0,0 });
	spriteSpotlight.setTexture(theTextureMgr->getTexture("Spotlight"));
	spriteSpotlight.setSpriteDimensions(theTextureMgr->getTexture("Spotlight")->getTWidth(), theTextureMgr->getTexture("Spotlight")->getTHeight());

	spriteBkgd.setSpritePos({ 0,0 });
	spriteBkgd.setTexture(theTextureMgr->getTexture("theBackground"));
	spriteBkgd.setSpriteDimensions(theTextureMgr->getTexture("theBackground")->getTWidth(), theTextureMgr->getTexture("theBackground")->getTHeight());

	rosesCollected = 0;
	strScore = gameTextContent[4];
	strScore += to_string(rosesCollected).c_str();
	

	
	numTableItems = 0;
	theHSTable.loadFromFile("Data/HighScore.dat");

	theHighScoreTable = gameTextContent[6];

	theHSTableSize = theHSTable.getTableSize();

	highScoreTextures = { "score1", "score2", "score3", "score4", "score5", "score6", "score7", "score8", "score9", "score10" };

	for (int i = 0; i < theHSTableSize; i++)
	{
		string entry = "";
		entry += theHSTable.getItem(i).Name + " " + to_string(theHSTable.getItem(i).score);
		theTextureMgr->addTexture(highScoreTextures[i], theFontMgr->getFont("Walkway")->createTextTexture(theRenderer, entry.c_str(), textType::solid, { 44, 203, 112, 255 }, { 0, 0, 0, 0 }));
	}
	
	gameOver = false;
	

	
	


	





}











void cGame::run(SDL_Window* theSDLWND, SDL_Renderer* theRenderer)
{

	
	 loop = true;

	

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
	
	//playerSprite is rendered by using the current frameDrawn this allows for things such as idle animation to be added later.
	//Note the render order: This is so that the spotlight is rendered infront of Michael to allow the illusion of light as he passes through it.

	SDL_RenderClear(theRenderer);

	switch (theGameState)
	{
	
	case gameState::menu:
	{
		


		spriteBkgd.setTexture(theTextureMgr->getTexture("theBackground"));
		spriteBkgd.setSpriteDimensions(theTextureMgr->getTexture("theBackground")->getTWidth(), theTextureMgr->getTexture("theBackground")->getTHeight());
		spriteBkgd.render(theRenderer, NULL, NULL, spriteBkgd.getSpriteScale());

		playerSprite[frameDrawn].render(theRenderer, &playerSprite[frameDrawn].getSpriteDimensions(), &playerSprite->getSpritePos(), NULL, &playerSprite[frameDrawn].getSpriteCentre(), playerSprite[frameDrawn].getSpriteScale(), flipType);


		theButtonMgr->getBtn("Start")->setSpritePos({ 350,200 });
		theButtonMgr->getBtn("Start")->render(theRenderer, &theButtonMgr->getBtn("Start")->getSpriteDimensions(), &theButtonMgr->getBtn("Start")->getSpritePos(), theButtonMgr->getBtn("Start")->getSpriteScale());
		theButtonMgr->getBtn("Exit")->setSpritePos({ 350,300 });
		theButtonMgr->getBtn("Exit")->render(theRenderer, &theButtonMgr->getBtn("Exit")->getSpriteDimensions(), &theButtonMgr->getBtn("Exit")->getSpritePos(), theButtonMgr->getBtn("Exit")->getSpriteScale());


		tempTexture = theTextureMgr->getTexture("Title");
		pos = { 300 , 100 , tempTexture->getTextureRect().w , tempTexture->getTextureRect().h };
		scale = { 1,1 };
		tempTexture->renderTexture(theRenderer, tempTexture->getTexture(), &tempTexture->getTextureRect(), &pos, scale);

		tempTexture = theTextureMgr->getTexture("Collect");
		pos = { 30,500, tempTexture->getTextureRect().w , tempTexture->getTextureRect().h };
		tempTexture->renderTexture(theRenderer, tempTexture->getTexture(), &tempTexture->getTextureRect(), &pos, scale);

		tempTexture = theTextureMgr->getTexture("Avoid");
		pos = { 570 , 500 , tempTexture->getTextureRect().w , tempTexture->getTextureRect().h };
		tempTexture->renderTexture(theRenderer, tempTexture->getTexture(), &tempTexture->getTextureRect(), &pos, scale);

		tempTexture = theTextureMgr->getTexture("Movement");
		pos = { 300 , 700 , tempTexture->getTextureRect().w , tempTexture->getTextureRect().h };
		tempTexture->renderTexture(theRenderer, tempTexture->getTexture(), &tempTexture->getTextureRect(), &pos, scale);

		
	}
	break;
	case gameState::playing:
	{
		spriteBkgd.setTexture(theTextureMgr->getTexture("theBackground"));
		spriteBkgd.setSpriteDimensions(theTextureMgr->getTexture("theBackground")->getTWidth(), theTextureMgr->getTexture("theBackground")->getTHeight());
		spriteBkgd.render(theRenderer, NULL, NULL, spriteBkgd.getSpriteScale());

		playerSprite[frameDrawn].render(theRenderer, &playerSprite[frameDrawn].getSpriteDimensions(), &playerSprite->getSpritePos(), NULL, &playerSprite[frameDrawn].getSpriteCentre(), playerSprite[frameDrawn].getSpriteScale(), flipType);

		spriteSpotlight.setTexture(theTextureMgr->getTexture("Spotlight"));
		spriteSpotlight.setSpriteDimensions(theTextureMgr->getTexture("Spotlight")->getTWidth(), theTextureMgr->getTexture("Spotlight")->getTHeight());
		spriteSpotlight.render(theRenderer, NULL, NULL, spriteBkgd.getSpriteScale());

		theTextureMgr->addTexture("Score", theFontMgr->getFont("Walkway")->createTextTexture(theRenderer, strScore.c_str(), textType::solid, { 44, 203, 112, 255 }, { 0, 0, 0, 0 }));
		tempTexture = theTextureMgr->getTexture("Score");
		pos = { 60, 10 , tempTexture->getTextureRect().w , tempTexture->getTextureRect().h };
		tempTexture->renderTexture(theRenderer, tempTexture->getTexture(), &tempTexture->getTextureRect(), &pos, scale);
		
		theButtonMgr->getBtn("Exit")->setSpritePos({ 700,650 });
		theButtonMgr->getBtn("Exit")->render(theRenderer, &theButtonMgr->getBtn("Exit")->getSpriteDimensions(), &theButtonMgr->getBtn("Exit")->getSpritePos(), theButtonMgr->getBtn("Exit")->getSpriteScale());

	}
	break;

	case gameState::highscore:
	{
		spriteBkgd.render(theRenderer, NULL, NULL, spriteBkgd.getSpriteScale());

		tempTexture = theTextureMgr->getTexture("HighScore");
		pos = { 500,100 ,tempTexture->getTextureRect().w , tempTexture->getTextureRect().h };
		scale = { 1,1 };
		tempTexture->renderTexture(theRenderer, tempTexture->getTexture(), &tempTexture->getTextureRect(), &pos, scale);
		pos = { 500 , 200 , tempTexture->getTextureRect().w , tempTexture->getTextureRect().h };

		for (int i = 0; i < theHSTableSize; i++)
		{
			tempTexture = theTextureMgr->getTexture(highScoreTextures[i]);
			tempTexture->renderTexture(theRenderer, tempTexture->getTexture(), &tempTexture->getTextureRect(), &pos, scale);
			pos = { 500 , 200 + (50 * i), tempTexture->getTextureRect().w , tempTexture->getTextureRect().h };
		}

		theButtonMgr->getBtn("Replay")->setSpritePos({ 0, 0 });
		theButtonMgr->getBtn("Replay")->render(theRenderer, &theButtonMgr->getBtn("Replay")->getSpriteDimensions(), &theButtonMgr->getBtn("Replay")->getSpritePos(), theButtonMgr->getBtn("Replay")->getSpriteScale());

		theButtonMgr->getBtn("Exit")->setSpritePos({ 0, 0 });
		theButtonMgr->getBtn("Exit")->render(theRenderer, &theButtonMgr->getBtn("Exit")->getSpriteDimensions(), &theButtonMgr->getBtn("Exit")->getSpritePos(), theButtonMgr->getBtn("Exit")->getSpriteScale());

	}

	break;
	case gameState::end:
	{
		loop = false;
	}
	break;
	default:
	{
		break;

	}


	}
	
	SDL_RenderPresent(theRenderer);
}

void cGame::render(SDL_Window* theSDLWND, SDL_Renderer* theRenderer, double rotAngle, SDL_Point* spriteCentre)
{

	SDL_RenderPresent(theRenderer);
}

void cGame::update(double deltaTime)
{
	if (buttonPressed == false)
	{

		frameDrawn = 8;
		frameDrawn = ((SDL_GetTicks() - startTime) * 2 / 1000) % 4 + 8;

		cout << frameDrawn << " ";


		if (frameDrawn > 12 || frameDrawn < 8) { frameDrawn = 8; }


	}

	if (theGameState == gameState::playing)
	{
		newPos = { playerSprite->getSpritePos().x , playerSprite->getSpritePos().y };
	}


	if (theGameState == gameState::menu || theGameState == gameState::end )
	{
		theGameState = theButtonMgr->getBtn("Exit")->update(theGameState, gameState::quit, mouseClicked);
	}
	else
	{
		theGameState = theButtonMgr->getBtn("Exit")->update(theGameState, gameState::end, mouseClicked);
	}


	if (theGameState == gameState::highscore)
	{
		theGameState = theButtonMgr->getBtn("Exit")->update(theGameState, gameState::quit, mouseClicked);
		theGameState = theButtonMgr->getBtn("Replay")->update(theGameState, gameState::menu, mouseClicked);
	}


	if (theGameState == gameState::menu)
	{
		theGameState = theButtonMgr->getBtn("Start")->update(theGameState, gameState::playing, mouseClicked);
		gameOver = false;
		if (theGameState == gameState::playing && gameOver == false)
		{
			theGameState = theButtonMgr->getBtn("Start")->update(theGameState, gameState::playing, mouseClicked);
			rosesCollected = 0;

		}

	}

	if (theGameState == gameState::playing)
	{


		if(gameOver)
		{
			theGameState = gameState::end;
		}
	}


}

void cGame::update()
{

}

/*
	Play music checks to see if the sound will play before allowing it to play

	This was created with reference to : http://lazyfoo.net/SDL_tutorials/

*/
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


/*	When the bool buttonPressed is false then i sets the current frameDrawn to the fitst frame of the idle animation and it
	loops through it until one of the movement keys are pressed and then the bool is true.
*/
bool cGame::getInput(bool theLoop)
{
		
	//Set newPos as current position
	
	
	SDL_Event event;

	while (SDL_PollEvent(&event))
	{
		if (event.type == SDL_QUIT)
		{
			theLoop = false;
		}

		/*
			Checks for key input, changes the sprites render flip depending on orientation,	
			changes the player position and moves through the frames of animation.
			
		
		*/

		keystate = SDL_GetKeyboardState(NULL);

		if (keystate[SDL_SCANCODE_D] && theGameState == gameState::playing)
		{
			
			flipType = SDL_FLIP_NONE;
			frameDrawn = ((SDL_GetTicks() - startTime) * animRate / 1000) % animLength; //Sets the frame drawn to the next.

			newPos.x += 3;	//Change player position
			playerSprite->setSpritePos(newPos);
			
			if (frameDrawn >= 7) { frameDrawn = 0; } //Limit of animation
			
			cout << frameDrawn << endl;
		
		}


		if (keystate[SDL_SCANCODE_A] && theGameState == gameState::playing)
		{
			
			flipType = SDL_FLIP_HORIZONTAL;
			frameDrawn = ((SDL_GetTicks() - startTime) * animRate / 1000) % animLength;

			newPos.x -= 3;
			playerSprite->setSpritePos(newPos);
			
			if (frameDrawn >= 7) { frameDrawn = 0; }
			
			cout << frameDrawn << endl;
			
		}

		//If the key is released then the idle animation plays
		
		if (event.type == SDL_KEYUP && theGameState == gameState::playing)
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

		if (event.type == SDL_MOUSEBUTTONDOWN)
		{
			if (SDL_BUTTON_LEFT)
			{
				mouseClicked = { event.motion.x , event.motion.y };

			}
		}


		//When the key is pressed the walking animation plays and the idle stops.
		if (event.type == SDL_KEYDOWN && theGameState == gameState::playing)
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

	
		//Makes the player wrap around the screen. To be honest the simplest part yet my favourite. :D

		if ((newPos.x > 1024))
		{
			newPos.x = -49;
		}
		if ((newPos.x < -50))
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

	TTF_Quit();

	// Quit IMG system
	IMG_Quit();

	// Shutdown SDL 2
	SDL_Quit();
}

