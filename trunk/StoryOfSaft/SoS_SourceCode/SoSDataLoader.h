/*
	Author: Richard McKenna
			Stony Brook University
			Computer Science Department

	SoSDataLoader.h

	This class provides a custom importer for the SoSGame to import
	game data, gui data, and world (i.e. level) data.
*/
#pragma once
#include "stdafx.h"
#include "SSSF_SourceCode\dataLoader\GameDataLoader.h"
#include "SSSF_SourceCode\game\Game.h"
#include "SSSF_SourceCode\PlatformPlugins\DirectXPlugin\DirectXTextureManager.h"

class SoSDataLoader : public GameDataLoader
{
private:
	// WE NEED THESE GUYS TO INIT OUR WINDOWS WINDOW
	HINSTANCE hInstance;
	int nCmdShow;
	
	//helper method for loading screens from file
	//(no good way to convert wstring to enum)
	GameState gsLookup(wstring g);

	//helper method for loading world
	void loadPlayer(Game *game, wstring playerInitFile);
	void loadBots(Game *game, wstring botInitFile);
	void loadCodeLocs(Game *game, wstring codLocInitFile);

public:
	SoSDataLoader()	{}
	~SoSDataLoader()	{}

	// INLINED MUTATOR METHOD
	void initWinHandle(HINSTANCE initHInstance, int initNCmdShow)
	{
		hInstance = initHInstance;
		nCmdShow = initNCmdShow;
	}


	// THESE SHOULD BE LOADING DATA FROM FILES. THEY
	// ARE ALL DEFINED IN SoSDataLoader.cpp
	void loadGame(Game *game, wstring gameInitFile);
	void loadGUI(Game *game, wstring guiInitFile);
	void loadWorld(Game *game, wstring levelInitFile);


	// THESE ARE HARD-CODED EXAMPLES OF GAME DATA LOADING
	void hardCodedLoadGUIExample(Game *game);
	void hardCodedLoadLevelExample(Game *game);

	// SOME HELPER METHODS FOR SETTING UP THE GAME
	void initColors(GameGraphics *graphics,	map<wstring,wstring> *properties);
	void initViewport(GameGUI *gui, map<wstring,wstring> *properties);

	// AND SOME HELPER METHODS FOR LOADING THE GUI
	void initCursor(GameGUI *gui, DirectXTextureManager *guiTextureManager);
	void initSplashScreen(Game *game, GameGUI *gui,	DirectXTextureManager *guiTextureManager);
	void initMainMenu(GameGUI *gui,	DirectXTextureManager *guiTextureManager);
	void initInGameGUI(GameGUI *gui, DirectXTextureManager *guiTextureManager);
};