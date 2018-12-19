/*
==================================================================================
cGame.cpp
==================================================================================
*/

#include "cGame.h"
#include <iostream>
#include <fstream> 


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
Mix_Chunk* oof = NULL;
Mix_Chunk* heehee = NULL;
Mix_Chunk* wow = NULL;

 
random_device rd;
mt19937 gen{ rd() };
uniform_int_distribution<> roseDist{ 1, 10 };
uniform_int_distribution<> roseRot{ 0,360 };

const int JOYSTICK_DEADZONE = 8000;
SDL_Joystick* gameController = NULL;



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

	//The sounds are initialised and checked to see if they have been created. if not an error is diplayed
	playMusic();

	wow = Mix_LoadWAV("Sounds/wow.wav");
	heehee = Mix_LoadWAV("Sounds/heehee.wav");
	oof = Mix_LoadWAV("Sounds/oof.wav");
	if (oof == NULL)
	{
		cout << "oof sound failed to load" << Mix_GetError();
	}
	if (heehee == NULL)
	{
		cout << "heehee sound failed to load" << Mix_GetError();
	}
	if (wow == NULL)
	{
		cout << "wow sound failed to load" << Mix_GetError();
	}

	//Joystick is init and checked to see if it was init successfully, if not then an error is displayed.
	if (!SDL_INIT_JOYSTICK)
	{
		cout << "Joystick cannot be init error: " << SDL_GetError();
 	}

	if (SDL_NumJoysticks() < 1)
	{
		printf("Warning no joystick connected!");
	}
	else
	{
		gameController = SDL_JoystickOpen(0);
		if (gameController == NULL)
		{
			printf("Unable to open game controller!", SDL_GetError());
		}
	}
	
	

	rosesSpawned = 0;
	enemiesSpawned = 0;
	//mouse click position is initialised.
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

	//Fonts are created and properties of each are assigned

	fontList = { "Harlow","Walkway" };
	fontToUse = { "HARLOWSI.ttf" , "Walkway_UltraBold_0.ttf" };

	for (unsigned int i = 0; i < fontList.size(); i++)
	{
		if(i == 0){ theFontMgr->addFont(fontList[i], fontToUse[i], 48); }
		if(i==1){ theFontMgr->addFont(fontList[i], fontToUse[i], 24); }
	}


	//in game text is created and liked to an easy to remember name
	gameText = { "Title" , "Avoid" , "Collect" , "Movement" , "Score" , "EndScreen" , "HighScore" , "HighScoreAcheived" };
	gameTextContent = { "That Kid is Not My Son" , "Avoid babies: Those kids aren't your son!" , "Collect roses from your fans!" , "Use the A and D keys to move" , "Score: " , "But she claims that she is the one... ", "High score: " , "Congradulations highscore acheieved!"};

	for (unsigned int  i = 0; i < gameText.size(); i++)
	{
		if (i == 0 || i == 4 || i == 6 || i == 7)
		{
			theTextureMgr->addTexture(gameText[i], theFontMgr->getFont("Harlow")->createTextTexture(theRenderer, gameTextContent[i], textType::solid, { 242,82,173,255 }, { 0, 0, 0, 0 }));
		}
		else
		{
			theTextureMgr->addTexture(gameText[i], theFontMgr->getFont("Walkway")->createTextTexture(theRenderer, gameTextContent[i], textType::solid, { 44, 203, 112, 255 }, { 0, 0, 0, 0 }));

		}

	}

	//buttons created and makeshift positions created
	btnNameList = { "Start", "Exit" , "Replay" };
	btnTexturesToUse = { "Images/Play.png", "Images/Quit.png" , "Images/Retry.png" };
	btnPos = { {100,50} , {100,90} , {30,40} };

	for (unsigned int i = 0; i < btnNameList.size(); i++)
	{
		//texture created
		theTextureMgr->addTexture(btnNameList[i], btnTexturesToUse[i]);
	}
	for (unsigned int i = 0; i < btnNameList.size(); i++)
	{
		//texture assigned to buttons for later use
		cButton * newBtn = new cButton();
		newBtn->setTexture(theTextureMgr->getTexture(btnNameList[i]));
		newBtn->setSpritePos(btnPos[i]);
		newBtn->setSpriteDimensions(theTextureMgr->getTexture(btnNameList[i])->getTWidth(), theTextureMgr->getTexture(btnNameList[i])->getTHeight());
		theButtonMgr->add(btnNameList[i], newBtn);
	}
	
	//the initial gamestate set as menu so on startup the game begins at the menu
	theGameState = gameState::menu;
	

	//Populate the vectors provided with names and file locations.
	textureName = { "Player0","Player1","Player2","Player3","Player4","Player5","Player6","Player7","Player8","Player9","Player10","Player11", "Player12", "theBackground" , "Spotlight", "Rose", "Baby" };
	texturesToUse = { "Images/Michael/Moon0.png","Images/Michael/Moon1.png","Images/Michael/Moon2.png","Images/Michael/Moon3.png","Images/Michael/Moon4.png","Images/Michael/Moon5.png","Images/Michael/Moon6.png","Images/Michael/Moon7.png","Images/Michael/Nod0.png","Images/Michael/Nod1.png","Images/Michael/Nod2.png","Images/Michael/Nod3.png", "Images/Michael/SpinKick9.png", "Images/Backdrop.png" , "Images/Spotlight.png", "Images/Rose.png","Images/Baby.png" };
	//Add useable textures and apply a name to them using the image locations provided.
	for (unsigned int tCount = 0; tCount < textureName.size(); tCount++)
	{
		//assign the texture to a easy to remember name. Also the name is tied to a file location which will be the texture. 
		theTextureMgr->addTexture(textureName[tCount], texturesToUse[tCount]);
	}


	//Add the useable textures in populating the player sprite array for animation.
	for (int i = 0; i <= 12;i++)
	{
		playerSprite[i].setTexture(theTextureMgr->getTexture(textureName.at(i)));
		
	}
	
	//Set the first sprite frames position as the rest of the sprites inherit it.
	playerSprite[0].setSpritePos({ 500,525 });

	//Scale all the sprites
	playerScale.X, playerScale.Y = 1;
	playerSprite->setSpriteScale(playerScale);
	playerSprite->scaleSprite();

	//Create spotlight and background. Sets position of both
	spriteSpotlight.setSpritePos({ 0,0 });
	spriteSpotlight.setTexture(theTextureMgr->getTexture("Spotlight"));
	spriteSpotlight.setSpriteDimensions(theTextureMgr->getTexture("Spotlight")->getTWidth(), theTextureMgr->getTexture("Spotlight")->getTHeight());

	spriteBkgd.setSpritePos({ 0,0 });
	spriteBkgd.setTexture(theTextureMgr->getTexture("theBackground"));
	spriteBkgd.setSpriteDimensions(theTextureMgr->getTexture("theBackground")->getTWidth(), theTextureMgr->getTexture("theBackground")->getTHeight());

	//Set the number of roses collected to 0 so that the score doesnt carry over.
	rosesCollected = 0;

	//init the score shown at the top of the screen during play to be updated with the current number of roses collected (in this case 0)
	strScore = gameTextContent[4];
	strScore += to_string(rosesCollected).c_str();
	theTextureMgr->deleteTexture("Score");
	
	

	//Create table entries for the highscore menu

	//Init number of items on table
	numTableItems = 0;
	//load file for use
	theHSTable.loadFromFile("Data/HighScore.dat");

	

	theHSTableSize = theHSTable.getTableSize();

	//Highscore texture names
	highScoreTextures = { "score1", "score2", "score3", "score4", "score5", "score6", "score7", "score8", "score9", "score10" };

	//Find set and create the textures of the highscores from file using the highscoreManager class
	for (int i = 0; i < theHSTableSize; i++)
	{
		
		string entry = "";
		entry += theHSTable.getItem(i).Name + " " + to_string(theHSTable.getItem(i).score);
		theTextureMgr->addTexture(highScoreTextures[i], theFontMgr->getFont("Walkway")->createTextTexture(theRenderer, entry.c_str(), textType::solid, { 44, 203, 112, 255 }, { 0, 0, 0, 0 }));
		
		
	}
	
	//set gameover to false so the game doesnt close on startup
	gameOver = false;
	

	//Creating and assigning the rose collectibles properties.
	for (int i = 0; i < 7;i++)
	{
		//Create a new rose
		theRoses.push_back(new cFall);
		//Set the random position above the play window
		theRoses[i]->setSpritePos({ 200 * roseDist(gen), -80 * roseDist(gen) });
		//Set the falling velocity
		theRoses[i]->setSpriteTranslation({ 1 , -150 });
		
		//generate a random rotation
		theRoses[i]->setSpriteRotAngle(roseRot(gen));
		//Set the rose a texture (which is the rose texture)
		theRoses[i]->setTexture(theTextureMgr->getTexture("Rose"));
		//Set the dimentions to avoid odd rendering
		theRoses[i]->setSpriteDimensions(theTextureMgr->getTexture("Rose")->getTWidth(), theTextureMgr->getTexture("Rose")->getTHeight());
		//set the rate in which it falls
		theRoses[i]->setSpriteVel(200);
		//set the rose so that it can be seen
		theRoses[i]->setActive(true);


		//The babies are created in a similar way as above (See lines 242-258)
		theBabies.push_back(new cFall);
		theBabies[i]->setSpritePos({ 200 * roseDist(gen), -80 * roseDist(gen) });
		theBabies[i]->setSpriteTranslation({ 1 , -150 });

		theBabies[i]->setSpriteRotAngle(roseRot(gen));
		theBabies[i]->setTexture(theTextureMgr->getTexture("Baby"));
		theBabies[i]->setSpriteDimensions(theTextureMgr->getTexture("Baby")->getTWidth(), theTextureMgr->getTexture("Baby")->getTHeight());
		theBabies[i]->setSpriteVel(200);
		theBabies[i]->setActive(true);
	}
	

	
	


	





}










//This section of code keeps track of time within the game to allow for things such as animation or timers.

void cGame::run(SDL_Window* theSDLWND, SDL_Renderer* theRenderer)
{

	
	 loop = true;

	
	 //Plays the music on startup
	
	while (loop)
	{
	
		
		//We get the time that passed since the last frame
		double elapsedTime = this->getElapsedSeconds();

		loop = this->getInput(loop);
		
		this->update(elapsedTime);
		this->render(theSDLWND, theRenderer);
		
	}
	
}




bool resetVars = false;

void cGame::render(SDL_Window* theSDLWND, SDL_Renderer* theRenderer)
{
	
	//playerSprite is rendered by using the current frameDrawn this allows for things such as idle animation to be added later.
	//Note the render order: This is so that the spotlight is rendered infront of Michael to allow the illusion of light as he passes through it.

	SDL_RenderClear(theRenderer);

	switch (theGameState)
	{
	
	case gameState::menu:
	{
		
		//This is applied so that the score doest carry over on retry
		


		//Set the position and scale of the texture and render it
		spriteBkgd.setTexture(theTextureMgr->getTexture("theBackground"));
		spriteBkgd.setSpriteDimensions(theTextureMgr->getTexture("theBackground")->getTWidth(), theTextureMgr->getTexture("theBackground")->getTHeight());
		spriteBkgd.render(theRenderer, NULL, NULL, spriteBkgd.getSpriteScale());
		spriteSpotlight.setActive(false);

		//Set the position and frame of animation to draw the player sprite
		playerSprite[frameDrawn].setSpritePos({ 500,525 });
		playerSprite[frameDrawn].render(theRenderer, &playerSprite[frameDrawn].getSpriteDimensions(), &playerSprite->getSpritePos(), NULL, &playerSprite[frameDrawn].getSpriteCentre(), playerSprite[frameDrawn].getSpriteScale(), SDL_FLIP_NONE);

		
		//Set the positions of buttons and render them
		theButtonMgr->getBtn("Start")->setSpritePos({ 350,200 });
		theButtonMgr->getBtn("Start")->render(theRenderer, &theButtonMgr->getBtn("Start")->getSpriteDimensions(), &theButtonMgr->getBtn("Start")->getSpritePos(), theButtonMgr->getBtn("Start")->getSpriteScale());
		theButtonMgr->getBtn("Exit")->setSpritePos({ 350,300 });
		theButtonMgr->getBtn("Exit")->render(theRenderer, &theButtonMgr->getBtn("Exit")->getSpriteDimensions(), &theButtonMgr->getBtn("Exit")->getSpritePos(), theButtonMgr->getBtn("Exit")->getSpriteScale());



		//Set the position of text in the window and render
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

		rosesCollected = 0;
		strScore = gameTextContent[4];
		strScore += to_string(rosesCollected).c_str();
		


		theTextureMgr->addTexture("Score", theFontMgr->getFont("Harlow")->createTextTexture(theRenderer, strScore.c_str(), textType::solid, { 44, 203, 112, 255 }, { 0, 0, 0, 0 }));
	}
	break;
	case gameState::playing:
	{
		
		
		//Render the background
		spriteBkgd.setTexture(theTextureMgr->getTexture("theBackground"));
		spriteBkgd.setSpriteDimensions(theTextureMgr->getTexture("theBackground")->getTWidth(), theTextureMgr->getTexture("theBackground")->getTHeight());
		spriteBkgd.render(theRenderer, NULL, NULL, spriteBkgd.getSpriteScale());

		//render the playersprite ontop of the background but behind the spotlight
		playerSprite[frameDrawn].render(theRenderer, &playerSprite[frameDrawn].getSpriteDimensions(), &playerSprite->getSpritePos(), NULL, &playerSprite[frameDrawn].getSpriteCentre(), playerSprite[frameDrawn].getSpriteScale(), flipType);
		//Set the position of the spolight and render
		spriteSpotlight.setActive(true);
		spriteSpotlight.setTexture(theTextureMgr->getTexture("Spotlight"));
		spriteSpotlight.setSpriteDimensions(theTextureMgr->getTexture("Spotlight")->getTWidth(), theTextureMgr->getTexture("Spotlight")->getTHeight());
		spriteSpotlight.render(theRenderer, NULL, NULL, spriteBkgd.getSpriteScale());

		if (!resetVars)
		{
			theTextureMgr->deleteTexture("Score");
			resetVars = true;

		}
		
		//Update the score constantly so that when the score changes it shows
		
		theTextureMgr->addTexture("Score", theFontMgr->getFont("Harlow")->createTextTexture(theRenderer, strScore.c_str(), textType::solid, { 44, 203, 112, 255 }, { 0, 0, 0, 0 }));
		tempTexture = theTextureMgr->getTexture("Score");
		pos = { 60, 10 , tempTexture->getTextureRect().w , tempTexture->getTextureRect().h };
		tempTexture->renderTexture(theRenderer, tempTexture->getTexture(), &tempTexture->getTextureRect(), &pos, scale);

		//Render the roses and babies
		for (int draw = 0; draw < (int)theRoses.size(); draw++)
		{
			
			theRoses[draw]->render(theRenderer, &theRoses[draw]->getSpriteDimensions(), &theRoses[draw]->getSpritePos(), theRoses[draw]->getSpriteRotAngle(), &theRoses[draw]->getSpriteCentre(), theRoses[draw]->getSpriteScale(), SDL_FLIP_NONE);
			
		}

		for (int draw = 0; draw < (int)theBabies.size(); draw++)
		{
			
			theBabies[draw]->render(theRenderer, &theBabies[draw]->getSpriteDimensions(), &theBabies[draw]->getSpritePos(), theBabies[draw]->getSpriteRotAngle(), &theBabies[draw]->getSpriteCentre(), theBabies[draw]->getSpriteScale(), SDL_FLIP_NONE);

		}
		


		//Render an exit button for the player
		theButtonMgr->getBtn("Exit")->setSpritePos({ 700,650 });
		theButtonMgr->getBtn("Exit")->render(theRenderer, &theButtonMgr->getBtn("Exit")->getSpriteDimensions(), &theButtonMgr->getBtn("Exit")->getSpritePos(), theButtonMgr->getBtn("Exit")->getSpriteScale());

	}
	break;

	case gameState::highscore:
	{

		resetVars = false;

		for (int i = 0; i < (int)theBabies.size(); i++)
		{
			theBabies[i]->setSpritePos({ 200 * roseDist(gen), -80 * roseDist(gen) });
		}
		for (int i = 0; i < (int)theBabies.size(); i++)
		{
			theRoses[i]->setSpritePos({ 200 * roseDist(gen), -80 * roseDist(gen) });
		}

		playerSprite->setSpritePos({ 500,525 });

		//render the background
		spriteBkgd.render(theRenderer, NULL, NULL, spriteBkgd.getSpriteScale());

		//Render the player kicking a baby
		tempTexture = theTextureMgr->getTexture("Player12");
		pos = { 500,525 ,tempTexture->getTextureRect().w *2, tempTexture->getTextureRect().h *2};
		tempTexture->renderTexture(theRenderer, tempTexture->getTexture(), &tempTexture->getTextureRect(), &pos, { 2,2 });
		tempTexture = theTextureMgr->getTexture("Baby");
		pos = { 575,520 ,tempTexture->getTextureRect().w , tempTexture->getTextureRect().h };
		scale = { 1,1 };
		tempTexture->renderTexture(theRenderer, tempTexture->getTexture(), &tempTexture->getTextureRect(), &pos, scale);


		//Render the highscore text
		tempTexture = theTextureMgr->getTexture("HighScore");
		pos = { 600,100 ,tempTexture->getTextureRect().w , tempTexture->getTextureRect().h };
		scale = { 1,1 };
		tempTexture->renderTexture(theRenderer, tempTexture->getTexture(), &tempTexture->getTextureRect(), &pos, scale);
		
		//Render endscreen text
		tempTexture = theTextureMgr->getTexture("EndScreen");
		pos = { 100,400 ,tempTexture->getTextureRect().w , tempTexture->getTextureRect().h };
		scale = { 1,1 };
		tempTexture->renderTexture(theRenderer, tempTexture->getTexture(), &tempTexture->getTextureRect(), &pos, scale);



		//initialise variables for seeing if a highscore is acheived
		int currentScore;
		bool highScoreAcheived = false;
		int lineAcheived;
		//Open file for reading and writing
		fstream writeFile;
		writeFile.open("Data/HighScore.dat" , fstream::app);


		//Check the current score to the scores on the table
		for (int item = 0; item < theHSTableSize; item++)
		{
			currentScore = theHSTable.getItem(item).score;

			if (rosesCollected > currentScore)
			{
				//if the current score is greater than a score on the table set bool to true
				highScoreAcheived = true;
				lineAcheived = item;
			}

		}


		//Write a new score on the table 
		if (highScoreAcheived)
		{
			tempTexture = theTextureMgr->getTexture("HighScoreAcheived");
			pos = { 200,50 ,tempTexture->getTextureRect().w , tempTexture->getTextureRect().h };
			tempTexture->renderTexture(theRenderer, tempTexture->getTexture(), &tempTexture->getTextureRect(), &pos, scale);

			if (writeFile.is_open())
			{
				
				
				theHSTable.addItem({ "You",rosesCollected });
				theHSTable.saveToFile("Data/HighScore.dat");
				
			}
			//close file

			writeFile.close();


		}

		//Generate and display scores from the file
		pos = { 600, 150 , tempTexture->getTextureRect().w, tempTexture->getTextureRect().h };
		scale = { 1,1 };
		for (int item = 0; item < theHSTableSize; item++)
		{
		
			tempTexture = theTextureMgr->getTexture(highScoreTextures[item]);
			tempTexture->renderTexture(theRenderer, tempTexture->getTexture(), &tempTexture->getTextureRect(), &pos, scale);
			pos = pos = { 600, 200 + (50 * item), tempTexture->getTextureRect().w, tempTexture->getTextureRect().h };
		}


		//Render buttons
		theButtonMgr->getBtn("Replay")->setSpritePos({ 0, 650 });
		theButtonMgr->getBtn("Replay")->render(theRenderer, &theButtonMgr->getBtn("Replay")->getSpriteDimensions(), &theButtonMgr->getBtn("Replay")->getSpritePos(), theButtonMgr->getBtn("Replay")->getSpriteScale());

		theButtonMgr->getBtn("Exit")->setSpritePos({ 700, 650 });
		theButtonMgr->getBtn("Exit")->render(theRenderer, &theButtonMgr->getBtn("Exit")->getSpriteDimensions(), &theButtonMgr->getBtn("Exit")->getSpritePos(), theButtonMgr->getBtn("Exit")->getSpriteScale());

	}

	break;
	//When the game loop is false the game closes
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
	//Since my playerSrite is set in an array this is how the collider is set and updated
	
	
	//While no buttons are pressed, play the idle animation
	if (buttonPressed == false)
	{

		//Set the first frame
		frameDrawn = 8;
		//Set the rate in which the frames change
		frameDrawn = ((SDL_GetTicks() - startTime) * 2 / 1000) % 4 + 8;

		cout << frameDrawn << " ";

		//Set frame limits so that it doesnt play the wrong animation.
		if (frameDrawn > 12 || frameDrawn < 8) { frameDrawn = 8; }


	}

	if (theGameState == gameState::playing)
	{
		//Set an iterator to constantly go through each element of the vector
		vector<cFall*>::iterator roseIterator = theRoses.begin();
		vector<cFall*>::iterator babyIterator = theBabies.begin();
		//These are used later for position changes and collision
		int currentBaby = 0;
		int currentRose = 0;
		//While the iterator isnt at the end of the vector 
		while (roseIterator != theRoses.end())
		{
			//If the rose is not active erase it
			if ((*roseIterator)->isActive() == false)
			{
				roseIterator = theRoses.erase(roseIterator);
			
			}
			//Otherewise move on and update the position/time of the rose
			
			{
				(*roseIterator)->update(deltaTime);
				++roseIterator;
				++currentRose;
			}
			//If the current rose is past the position on screen select a random poition at the top of the screen
			if (currentRose >= 6) { currentRose = 0; }
			if (theRoses[currentRose]->getSpritePos().y > 700)
			{

				theRoses[currentRose]->setSpritePos({ 75 * roseDist(gen), -100 * roseDist(gen) });
			}

		}
		//This is similar code to the rose iterator ( see lines 579 - 600)
		while (babyIterator != theBabies.end())
		{

			if ((*babyIterator)->isActive() == false)
			{
				babyIterator = theBabies.erase(babyIterator);

			}
			else
			{
				(*babyIterator)->update(deltaTime);
				++babyIterator;
				++currentBaby;
			}
			if (currentBaby >= 6) { currentBaby = 0; }
			if (theBabies[currentBaby]->getSpritePos().y > 800)
			{

				theBabies[currentBaby]->setSpritePos({ 75 * roseDist(gen), -100 * roseDist(gen) });
			}

		}


		//This is for collision checking
		//If the rose collides with the player set active to false and move it to the top of the screen and then set it to active again
		for (vector<cFall*>::iterator roseIterator = theRoses.begin(); roseIterator != theRoses.end(); ++roseIterator)
		{
			if ((*roseIterator)->collidedWith(&(*roseIterator)->getBoundingRect(), &playerSprite->getBoundingRect()))
			{
				Mix_PlayChannel(-1, oof, 0);
				(*roseIterator)->setActive(false);
				(*roseIterator)->setSpritePos({ 200 * roseDist(gen), -150 * roseDist(gen) });
				(*roseIterator)->setActive(true);

				

				++rosesCollected;
				//Update the score
				strScore = gameTextContent[4];
				strScore += to_string(rosesCollected).c_str();
				theTextureMgr->deleteTexture("Score");
				

				
			}
		}


		//If the player collides with a baby, set the gamestate to the end/highscore screen
		for (vector<cFall*>::iterator babyIterator = theBabies.begin(); babyIterator != theBabies.end(); ++babyIterator)
		{
			if ((*babyIterator)->collidedWith(&(*babyIterator)->getBoundingRect(), &playerSprite->getBoundingRect()))
			{
				Mix_PlayChannel(-1, wow, 0);
				theGameState = gameState::highscore;

			}
		}

		
		//Update the position to prevent odd sprite positioning
		newPos = { playerSprite->getSpritePos().x , playerSprite->getSpritePos().y };
		
	}

	

	//This section is for cheching for mouse clicks on buttons and what to do when this occurs


	if (theGameState == gameState::menu || theGameState == gameState::highscore )
	{
		//If the player clicks on exit the gamestate changes to quit
		theGameState = theButtonMgr->getBtn("Exit")->update(theGameState, gameState::quit, mouseClicked);
		
	}
	else
	{
		//Failsafe that does the same job
		theGameState = theButtonMgr->getBtn("Exit")->update(theGameState, gameState::end, mouseClicked);
	}


	if (theGameState == gameState::highscore)
	{

		
		//If user clicks on replay, go back to the menu to retry.
		theGameState = theButtonMgr->getBtn("Replay")->update(theGameState, gameState::menu, mouseClicked);
		
		



	}


	if (theGameState == gameState::menu)
	{
		//Set button pressed to false to allow for the player to do the idle animation
		buttonPressed = false;
		//If the player clicks on the start button, set the gamestate to playing
		theGameState = theButtonMgr->getBtn("Start")->update(theGameState, gameState::playing, mouseClicked);
		//Game over set to false to prevent game ending
		gameOver = false;
		//score set to 0 to reset the player score
	

		
		if (theGameState == gameState::playing && gameOver == false)
		{
			theGameState = theButtonMgr->getBtn("Start")->update(theGameState, gameState::playing, mouseClicked);


		}

	}

	if (theGameState == gameState::playing)
	{
		//If gameOver is true end the game

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
	
	//Check to see if audio can be played
	if (Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096) == -1)
	{
		cout << "Mixer init error: " << Mix_GetError() << endl;
	}
	//Used for error chcking
	int flags = MIX_INIT_MP3;
	int result = 0;

	
	//If the init fails send to command prompt
	if (SDL_INIT_AUDIO < 1)
	{
		cout << "Failed to init audio" << Mix_GetError() << endl;
	}
	if (flags != (result = Mix_Init(flags)))
	{
		cout<< "Couldn't init mixer ERROR " << Mix_GetError() << endl;
	}
	
	
	//Play music in a loop
	Mix_PlayMusic(music, -1);
	cout << "Mixer INIT " << endl;


}


/*	When the bool buttonPressed is false then i sets the current frameDrawn to the fitst frame of the idle animation and it
	loops through it until one of the movement keys are pressed and then the bool is true.
*/
bool cGame::getInput(bool theLoop)
{
		
	//Set newPos as current position
	
	 xDir = 0;
	 yDir = 0;



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

		// The controller support in the main game.
		//if the joystick goes over the deadzone value then the xDir is changed depending on direction.
		//If it isnt being moved then the idle animation is activated.
		if (event.type == SDL_JOYAXISMOTION)
		{
			if (event.jaxis.which == 0)
			{
				if (event.jaxis.axis == 0)
				{
					if (event.jaxis.value < -JOYSTICK_DEADZONE)
					{
						xDir = -1;
					}
					else if (event.jaxis.value > JOYSTICK_DEADZONE)
					{
						xDir = 1;
					}
					else
					{
						xDir = 0;
						buttonPressed = false;
					}
				}

				//This isnt needed in the game but if there way anything needed for the y axis this would be implemented
				if (event.jaxis.axis == 1)
				{
					if (event.jaxis.value < -JOYSTICK_DEADZONE)
					{
						yDir = -1;
					}
					else if (event.jaxis.value > JOYSTICK_DEADZONE)
					{
						yDir = 1;
					}
					else
					{
						yDir = 0;
					}
				}

			}
		}


		keystate = SDL_GetKeyboardState(NULL);

		if (keystate[SDL_SCANCODE_D] && theGameState == gameState::playing || xDir == -1 && theGameState == gameState::playing)
		{
			
			flipType = SDL_FLIP_NONE;
			frameDrawn = ((SDL_GetTicks() - startTime) * animRate / 1000) % animLength; //Sets the frame drawn to the next.

			newPos.x += 3;	//Change player position
			playerSprite->setSpritePos(newPos);
			playerSprite->setBoundingRect(playerSprite->getBoundingRect());
			
			if (frameDrawn >= 7) { frameDrawn = 0; } //Limit of animation
			
			cout << frameDrawn << endl;
		
		}


		if (keystate[SDL_SCANCODE_A] && theGameState == gameState::playing || xDir == 1 && theGameState == gameState::playing)
		{
			
			flipType = SDL_FLIP_HORIZONTAL;
			frameDrawn = ((SDL_GetTicks() - startTime) * animRate / 1000) % animLength;

			newPos.x -= 3;
			playerSprite->setSpritePos(newPos);
			playerSprite->setBoundingRect(playerSprite->getBoundingRect());
			
			if (frameDrawn >= 7) { frameDrawn = 0; }
			
			cout << frameDrawn << endl;
			
		}

		//If the key is released then the idle animation plays
		
		if (event.type == SDL_KEYUP && theGameState == gameState::playing )
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
				Mix_PlayChannel(-1, heehee, 0);

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

	theTextureMgr->~cTextureMgr();
	
	Mix_FreeChunk(oof);
	Mix_FreeMusic(music);

	SDL_JoystickClose(gameController);
	gameController = NULL;
}

