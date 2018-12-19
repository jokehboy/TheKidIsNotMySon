#ifndef _GAME_H
#define _GAME_H

/*
==================================================================================
cGame.h
==================================================================================
*/


#include <SDL.h>
#include<SDL_mixer.h>
#include <random>
// Game specific includes
#include "rocketGame.h"


using namespace std;

class cGame
{
public:
	cGame();

	void initialise(SDL_Window* theSDLWND, SDL_Renderer* theRenderer);
	void run(SDL_Window* theSDLWND, SDL_Renderer* theRenderer);
	void cleanUp(SDL_Window* theSDLWND);
	void render(SDL_Window* theSDLWND, SDL_Renderer* theRenderer);
	void render(SDL_Window* theSDLWND, SDL_Renderer* theRenderer, double rotAngle, SDL_Point* spriteCentre);
	void update();
	void update(double deltaTime);
	bool getInput(bool theLoop);
	void move(Uint32 deltaTime);
	double getElapsedSeconds();
	

	

	void playMusic();


	static cGame* getInstance();

private:

	static cGame* pInstance;
	// for framerates
	time_point< high_resolution_clock > m_lastTime;
	time_point< high_resolution_clock > m_CurrentTime;
	duration< double > deltaTime;

	// game related variables
	// Sprites for displaying background and rocket textures
	cSprite spriteBkgd;
	cSprite spriteSpotlight;

	cFall Baby;
	cFall Rose;

	vector<cFall*> theBabies;
	vector<cFall*> theRoses;
	int rosesSpawned;
	int enemiesSpawned;

	cSprite playerSprite[100];
	
	vector<LPCSTR> textureName;
	vector<LPCSTR> textName;
	vector<LPCSTR> texturesToUse;
	cTexture* tempTexture;
	SDL_Rect tempRect;

	vector<LPCSTR> fontList;
	vector<LPCSTR> fontToUse;

	vector<LPCSTR> gameText;
	vector<LPCSTR> gameTextContent;
	vector<LPCSTR> highScoreTextures;

	vector<LPCSTR> btnNameList;
	vector<LPCSTR> btnTexturesToUse;
	vector<SDL_Point> btnPos;
	vector <cButton> theButtons;
	btnTypes theBtnType;

	vector<LPCSTR> soundList;
	vector<LPCSTR> soundToUse;
	vector<soundType> soundTypes;

	SDL_Point areaClicked;
	gameState theGameState;

	SDL_Rect pos;
	FPoint scale;

	string strScore;
	int rosesCollected;
	
	int xDir;
	int yDir;

	cHighScoreTable theHSTable;
	int numTableItems;
	string theHighScoreTable;
	int theHSTableSize;

	int renderWidth, renderHeight;

	SDL_Point mouseClicked;

	bool gameOver;
	bool loop;


};

#endif
