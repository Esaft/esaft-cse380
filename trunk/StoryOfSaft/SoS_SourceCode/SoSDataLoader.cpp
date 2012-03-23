#include "stdafx.h"

// DUMMY GAME INCLUDES
#include "SoS_SourceCode\SoSButtonEventHandler.h"
#include "SoS_SourceCode\SoSDataLoader.h"
#include "SoS_SourceCode\SoSGame.h"
#include "SoS_SourceCode\SoSKeyEventHandler.h"
#include "SoS_SourceCode\SoSTextGenerator.h"

// GAME OBJECT INCLUDES
#include "SSSF_SourceCode\game\Game.h"
#include "SSSF_SourceCode\game\GameRules.h"
#include "SSSF_SourceCode\graphics\GameGraphics.h"
#include "SSSF_SourceCode\gsm\state\GameState.h"
#include "SSSF_SourceCode\gsm\world\TiledLayer.h"
#include "SSSF_SourceCode\gui\Cursor.h"
#include "SSSF_SourceCode\gui\GameGUI.h"
#include "SSSF_SourceCode\gui\ScreenGUI.h"
#include "SSSF_SourceCode\input\GameInput.h"
#include "SSSF_SourceCode\os\GameOS.h"
#include "SSSF_SourceCode\text\GameText.h"
#include "SSSF_SourceCode\text\BufferedTextFileReader.h"
#include "SSSF_SourceCode\gsm\ai\SimplePatternBot.h"
#include "SSSF_SourceCode\gsm\ai\Bot.h"

// WINDOWS PLATFORM INCLUDES
#include "SSSF_SourceCode\PlatformPlugins\WindowsPlugin\WindowsOS.h"
#include "SSSF_SourceCode\PlatformPlugins\WindowsPlugin\WindowsInput.h"
#include "SSSF_SourceCode\PlatformPlugins\WindowsPlugin\WindowsTimer.h"

// DIRECTX INCLUDES
#include "SSSF_SourceCode\PlatformPlugins\DirectXPlugin\DirectXGraphics.h"
#include "SSSF_SourceCode\PlatformPlugins\DirectXPlugin\DirectXTextureManager.h"

/*
	loadGame - This method loads the setup game data into the game and
	constructs all the needed objects for the game to work.
*/
void SoSDataLoader::loadGame(Game *game, wstring gameInitFile)
{
	// AND LET'S READ IN THE GAME SETUP INFO
	// FIRST LOAD ALL THE PROPERTIES
	map<wstring,wstring> *properties = new map<wstring,wstring>();
	loadGameProperties(game, properties, gameInitFile);

	// WE NEED THE TITLE AND USE_FULLSCREEN_MODE TO INITIALIZE OUR WINDOW
	wstring titleProp = (*properties)[SoS_TITLE];
	wstring useFullscreenProp = (*properties)[SoS_USE_FULLSCREEN_MODE];
	bool useFullscreen = false;
	if (useFullscreenProp.compare(L"true") == 0)
		useFullscreen = true;

	// GET THE SCREEN WIDTH AND HEIGHT
	int screenWidth, screenHeight;
	wstring screenWidthProp = (*properties)[SoS_SCREEN_WIDTH];
	wstring screenHeightProp = (*properties)[SoS_SCREEN_HEIGHT];
	wstringstream(screenWidthProp) >> screenWidth;
	wstringstream(screenHeightProp) >> screenHeight;

	// MAKE A CUSTOM GameOS OBJECT (WindowsOS) FOR SOME WINDOWS
	// PLATFORM STUFF, INCLUDING A Window OF COURSE
	WindowsOS *sosOS = new WindowsOS(	hInstance, 
										nCmdShow,
										useFullscreen,
										titleProp,
										screenWidth, screenHeight,
										game);
	
	int textFontSize;
	wstring textFontSizeProp = (*properties)[SoS_TEXT_FONT_SIZE];
	wstringstream(textFontSizeProp) >> textFontSize;

	// RENDERING WILL BE DONE USING DirectX
	DirectXGraphics *sosGraphics = new DirectXGraphics(game);
	sosGraphics->init(screenWidth, screenHeight);
	sosGraphics->initGraphics(sosOS->getWindowHandle(), useFullscreen);
	sosGraphics->initTextFont(textFontSize);

	// AND NOW LOAD THE COLORS THE GRAPHICS WILL USE
	// AS A COLOR KEY AND FOR RENDERING TEXT
	initColors(sosGraphics, properties);

	// WE'LL USE WINDOWS PLATFORM METHODS FOR GETTING INPUT
	WindowsInput *sosInput = new WindowsInput();

	// AND WINDOWS FOR THE TIMER AS WELL
	WindowsTimer *sosTimer = new WindowsTimer();

	// NOW INITIALIZE THE Game WITH ALL THE
	// PLATFORM AND GAME SPECIFIC DATA WE JUST CREATED
	game->initPlatformPlugins(	(GameGraphics*)sosGraphics,
								(GameInput*)sosInput,
								(GameOS*)sosOS,
								(GameTimer*)sosTimer);

	// LOAD OUR CUSTOM TEXT GENERATOR, WHICH DRAWS
	// TEXT ON THE SCREEN EACH FRAME
	SoSTextGenerator *sosTextGenerator = new SoSTextGenerator();
	sosTextGenerator->initText(game);
	GameText *text = game->getText();
	text->initDebugFile(SoS_DEBUG_FILE);
	text->setTextGenerator((TextGenerator*)sosTextGenerator);

	// INIT THE VIEWPORT TOO
	initViewport(game->getGUI(), properties);	

	//loadBots(game, SoS_BOT_INIT_FILE);

	// WE DON'T NEED THE PROPERTIES MAP ANYMORE, THE GAME IS ALL LOADED
	delete properties;
}

/*
	initColors - this helper method loads the color key, used for loading
	images, and the font color, used for rendering text.
*/
void SoSDataLoader::initColors(	GameGraphics *graphics,
									map<wstring,wstring> *properties)
{
	int fontRed, fontGreen, fontBlue;
	wstring fontRedProp = (*properties)[SoS_FONT_COLOR_RED];
	wstring fontGreenProp = (*properties)[SoS_FONT_COLOR_GREEN];
	wstring fontBlueProp = (*properties)[SoS_FONT_COLOR_BLUE];
	wstringstream(fontRedProp) >> fontRed;
	wstringstream(fontGreenProp) >> fontGreen;
	wstringstream(fontBlueProp) >> fontBlue;

	// COLOR USED FOR RENDERING TEXT
	graphics->setFontColor(fontRed, fontGreen, fontBlue);

	int keyRed, keyGreen, keyBlue;
	wstring keyRedProp = (*properties)[SoS_COLOR_KEY_RED];
	wstring keyGreenProp = (*properties)[SoS_COLOR_KEY_GREEN];
	wstring keyBlueProp = (*properties)[SoS_COLOR_KEY_BLUE];
	wstringstream(keyRedProp) >> keyRed;
	wstringstream(keyGreenProp) >> keyGreen;
	wstringstream(keyBlueProp) >> keyBlue;

	// COLOR KEY - COLOR TO BE IGNORED WHEN LOADING AN IMAGE
	// NOTE, IF YOU WISH TO USE PNG IMAGES THAT CONTAIN ALPHA
	// CHANNEL DATA, YOU DON'T NEED A COLOR KEY
	graphics->setColorKey(keyRed, keyGreen, keyBlue);
}

/*
	loadGUI - This method loads all the GUI assets described in the guiInitFile
	argument. Note that we are loading all GUI art for all GUIs when the application
	first starts. We'll learn a better technique later in the semester.
*/
void SoSDataLoader::loadGUI(Game *game, wstring guiInitFile)
{
	

	
	///ACTUALLY LOADING FROM FILE
	GameGUI *gui = game->getGUI();
	GameGraphics *graphics = game->getGraphics();
	DirectXTextureManager *guiTextureManager = (DirectXTextureManager*)graphics->getGUITextureManager();

	map<wstring,wstring> *properties = new map<wstring,wstring>();
	loadGameProperties(game, properties, guiInitFile);
	
	//Set up cursor
	int numCursor;
	wstring numCursorProp = (*properties)[SoS_NUM_CURSORS];
	wstringstream(numCursorProp) >> numCursor;

	//if there are custom cursors
	if(numCursor > 0)
	{
		vector<unsigned int> *imageIDs = new vector<unsigned int>();
		int imageID;

		for(int i = 0; i < numCursor; i++)
		{
			wstring label = SoS_CURSOR_PATH;
			wstringstream wss;
			wss.str(L"");
			wss << i;
			label.append(wss.str());

			imageID = guiTextureManager->loadTexture(((*properties)[label]));
			imageIDs->push_back(imageID);
		}

		Cursor *cursor = new Cursor();
		cursor->initCursor(	imageIDs,
							*(imageIDs->begin()),
							0,
							0,
							0,
							255,
							32,
							32);
		gui->setCursor(cursor);
	}


	//Set up screens
	int numScreens;
	wstring numScreensProp = (*properties)[SoS_NUM_SCREENS];
	wstringstream(numScreensProp) >> numScreens;

	for(int i = 0; i < numScreens; i++)
	{
		ScreenGUI *screenToAdd = new ScreenGUI();
		wstringstream screenID;
		screenID.str(L"");
		screenID << i;

		int numImages;
		wstring numImagesProp = (*properties)[screenID.str().append(SoS_SCREEN_NUM_IMAGES)];
		wstringstream(numImagesProp) >> numImages;

		for(int j = 0; j < numImages; j++)
		{
			wstringstream overlayID;
			overlayID.str(L"");
			overlayID << j;

			wstring path = (*properties)[screenID.str().append(SoS_SCREEN_IMAGE_PATH).append(overlayID.str())];

			unsigned int imageID = guiTextureManager->loadTexture(path);

			OverlayImage *imageToAdd = new OverlayImage();

			
			int x,y,z,alpha,width,height;

			wstring xProp = (*properties)[screenID.str().append(SoS_SCREEN_IMAGE_X).append(overlayID.str())];
			wstring yProp = (*properties)[screenID.str().append(SoS_SCREEN_IMAGE_Y).append(overlayID.str())];
			wstring zProp = (*properties)[screenID.str().append(SoS_SCREEN_IMAGE_Z).append(overlayID.str())];
			wstring alphaProp = (*properties)[screenID.str().append(SoS_SCREEN_IMAGE_ALPHA).append(overlayID.str())];
			wstring widthProp = (*properties)[screenID.str().append(SoS_SCREEN_IMAGE_WIDTH).append(overlayID.str())];
			wstring heightProp = (*properties)[screenID.str().append(SoS_SCREEN_IMAGE_HEIGHT).append(overlayID.str())];
			wstringstream(xProp) >> x;
			wstringstream(yProp) >> y;
			wstringstream(zProp) >> z;
			wstringstream(alphaProp) >> alpha;
			wstringstream(widthProp) >> width;
			wstringstream(heightProp) >> height;
			
			imageToAdd->x = x;
			imageToAdd->y = y;
			imageToAdd->z = z;
			imageToAdd->alpha = alpha;
			imageToAdd->width = width;
			imageToAdd->height = height;
			imageToAdd->imageID = imageID;
			screenToAdd->addOverlayImage(imageToAdd);
		}

		int numButtons;
		wstring numButtonsProp = (*properties)[screenID.str().append(SoS_SCREEN_NUM_BUTTONS)];
		wstringstream(numButtonsProp) >> numButtons;
		for(int j = 0; j < numButtons; j++)
		{
			wstringstream buttonID;
			buttonID.str(L"");
			buttonID << j;

			wstring path = (*properties)[screenID.str().append(SoS_SCREEN_BUTTON_PATH).append(buttonID.str())];
			wstring pathMO = (*properties)[screenID.str().append(SoS_SCREEN_BUTTON_MO_PATH).append(buttonID.str())];

			unsigned int normalTextureID = guiTextureManager->loadTexture(path);
			unsigned int mouseOverTextureID;
			if(path.compare(pathMO) == 0)
				mouseOverTextureID = normalTextureID;
			else
				mouseOverTextureID = guiTextureManager->loadTexture(pathMO);

			int x,y,z,alpha,width,height;

			wstring xProp = (*properties)[screenID.str().append(SoS_SCREEN_BUTTON_X).append(buttonID.str())];
			wstring yProp = (*properties)[screenID.str().append(SoS_SCREEN_BUTTON_Y).append(buttonID.str())];
			wstring zProp = (*properties)[screenID.str().append(SoS_SCREEN_BUTTON_Z).append(buttonID.str())];
			wstring alphaProp = (*properties)[screenID.str().append(SoS_SCREEN_BUTTON_ALPHA).append(buttonID.str())];
			wstring widthProp = (*properties)[screenID.str().append(SoS_SCREEN_BUTTON_WIDTH).append(buttonID.str())];
			wstring heightProp = (*properties)[screenID.str().append(SoS_SCREEN_BUTTON_HEIGHT).append(buttonID.str())];
			wstringstream(xProp) >> x;
			wstringstream(yProp) >> y;
			wstringstream(zProp) >> z;
			wstringstream(alphaProp) >> alpha;
			wstringstream(widthProp) >> width;
			wstringstream(heightProp) >> height;

			bool mo = false;
			wstring moProp = (*properties)[screenID.str().append(SoS_SCREEN_BUTTON_MO).append(buttonID.str())];
			if(moProp.compare(L"true") == 0)
				mo = true;
			wstring com = (*properties)[screenID.str().append(SoS_SCREEN_BUTTON_COM).append(buttonID.str())];

			Button *buttonToAdd = new Button();
			buttonToAdd->initButton(normalTextureID, 
							mouseOverTextureID,
							x,
							y,
							z,
							alpha,
							width,
							height,
							mo,
							com);
			screenToAdd->addButton(buttonToAdd);
		}

		int numStates;
		wstring numStatesProp = (*properties)[screenID.str().append(SoS_SCREEN_NUM_STATES)];
		wstringstream(numStatesProp) >> numStates;
		for(int j = 0; j < numStates; j++)
		{
			wstringstream stateID;
			stateID.str(L"");
			stateID << j;

			GameState gs = gsLookup((*properties)[screenID.str().append(SoS_SCREEN_STATE).append(stateID.str())]);

			gui->addScreenGUI(gs,screenToAdd);
		}
	}

	delete properties;


	//initSplashScreen(game, gui, guiTextureManager);
	//initMainMenu(gui, guiTextureManager);
	//initInGameGUI(gui, guiTextureManager);

	// WE'RE JUST GOING TO IGNORE THE GUI FILE FOR NOW.
	// FOR THE MOMENT WE ARE CALLING THIS HARD-CODED GUI LOADER
	//hardCodedLoadGUIExample(game);
}





/*
	loadLevel - This method should load the data the level described by the
	levelInitFile argument in to the Game's game state manager.
*/
void SoSDataLoader::loadWorld(Game *game, wstring levelInitFile)	
{
	// NOTE:	I AM DEMONSTRATING HOW TO LOAD A LEVEL
	//			PROGRAMICALLY. YOU SHOULD DO THIS
	//			USING CSV FILES.
	//hardCodedLoadLevelExample(game);

	// FIRST SETUP THE GAME WORLD DIMENSIONS
	
	GameStateManager *gsm = game->getGSM();
	GameGraphics *graphics = game->getGraphics();
	GameRules *gR = game->getGameRules();
	TextureManager *worldTextureManager = graphics->getWorldTextureManager();
	Physics *physics = gsm->getPhysics();

	BufferedTextFileReader reader;
	reader.initFile(levelInitFile);
	wstring line;
	unsigned int delimiterIndex;
	unsigned int delimiterIndex2;
	wchar_t delim = ',';

	line = reader.getNextLine();
	physics->setGravity(_wtof(line.c_str()));


	line = reader.getNextLine();
	delimiterIndex = line.find(delim);
	int tWidth = _wtoi(line.substr(0, delimiterIndex).c_str());//tile width
	int tHeight = _wtoi(line.substr(delimiterIndex+1).c_str());//tile height

	line = reader.getNextLine();
	delimiterIndex = line.find(delim);
	int numColumns = _wtoi(line.substr(0, delimiterIndex).c_str());
	int numRows = _wtoi(line.substr(delimiterIndex+1).c_str());

	World *world = gsm->getWorld();
	world->setWorldWidth(numColumns * tWidth);
	world->setWorldHeight(numRows * tHeight);

	line = reader.getNextLine();
	delimiterIndex = line.find(delim);
	int vpX = _wtoi(line.substr(0, delimiterIndex).c_str());
	int vpY = _wtoi(line.substr(delimiterIndex+1).c_str());
	Viewport *viewport = game->getGUI()->getViewport();
	viewport->setViewportX(vpX);
	viewport->setViewportY(vpY);


	TiledLayer *tiledLayer = new TiledLayer(	numColumns,	numRows, 
												tWidth,		tHeight, 
												0, true, 
												numColumns * tWidth,
												numRows * tHeight);


	wstring tileInfoFile = reader.getNextLine();
	wstring playerInfoFile = reader.getNextLine();
	wstring tiledLayerFile = reader.getNextLine();
	wstring botInfoFile = reader.getNextLine();
	wstring codeLocInfoFile = reader.getNextLine();
	reader.closeReader();
	BufferedTextFileReader tileInfoReader;
	tileInfoReader.initFile(tileInfoFile);

	loadBots(game,botInfoFile);
	
	map<wstring, int> *tileIDMap = new map<wstring, int>();//Tile identifier, tileImageID
	map<wstring, int> *tileCollisionMap = new map<wstring, int>();//Tile identifier, tileImageID
	
	int numTiles = _wtoi(tileInfoReader.getNextLine().c_str());
	
	for(int i = 0; i < numTiles; i++)
	{
		line = tileInfoReader.getNextLine();
		delimiterIndex = line.find(delim);
		wstring name = line.substr(0, delimiterIndex);
		wstring path = line.substr(delimiterIndex+1);
		delimiterIndex = path.find(delim);
		int isCollidable =  _wtoi(path.substr(delimiterIndex+1).c_str());
		path = path.substr(0,delimiterIndex);
		(*tileIDMap)[name] = worldTextureManager->loadTexture(path);
		(*tileCollisionMap)[name] = isCollidable;
	}

	tileInfoReader.closeReader();
	BufferedTextFileReader tiledLayerReader;
	tiledLayerReader.initFile(tiledLayerFile);
	
	// LET'S GENERATE A BACKGROUND 
	for (int i = 0; i < numRows; i++)
	{
		line = tiledLayerReader.getNextLine();

		for (int j = 0; j < numColumns; j++)
		{
			delimiterIndex = line.find(delim);
			
			bool isCollidable = false;
			wstring tileName = line.substr(0,delimiterIndex);
			line = line.substr(delimiterIndex+1);
			int tileIDToUse = (*tileIDMap)[tileName];
			if((*tileCollisionMap)[tileName] == 1)
				isCollidable = true;

			Tile *tileToAdd = new Tile();
			tileToAdd->collidable = isCollidable;
			tileToAdd->textureID = tileIDToUse;
			tiledLayer->addTile(tileToAdd);
		}//end for
	}//end for
	tiledLayerReader.closeReader();
	
	world->addLayer(tiledLayer);
	
	delete tileIDMap;
	delete tileCollisionMap;

	loadPlayer(game, playerInfoFile);
	
	gR->setUpGame(game);
	
	
	SpriteManager *spriteManager = gsm->getSpriteManager();

	Bot* bot = gR->getBot(0);
	bot->setDead(false);

	AnimatedSpriteType* ast = bot->getSpriteType();

	bot->setAlpha(255);
	bot->setCurrentState(L"RIGHT_STATE");
	bot->setCurrentlyCollidable(true);
	bot->setIsStatic(false);
	PhysicalProperties *pp = bot->getPhysicalProperties();
	pp->setCollidable(true);
	pp->setGravAffected(true);
	pp->setX(400);
	pp->setY(1600);
	

	spriteManager->addBot(bot);

	bot = gR->getBot(2);

	bot->setAlpha(255);
	bot->setCurrentState(L"CODE_STATE");
	bot->setCurrentlyCollidable(true);
	bot->setIsStatic(false);
	pp = bot->getPhysicalProperties();
	pp->setCollidable(true);
	//pp->setGravAffected(true);
	pp->setX(600);
	pp->setY(1400);
	

	spriteManager->addBot(bot);
	/////////////////////////////////////////ADDING THE PATTERN BOT

	//SpriteManager *spriteManager = gsm->getSpriteManager();
	//wstring BOT_FLOATING0_IMG = L"./textures/world/sprites/hex/Hex0.png";
	//wstring BOT_FLOATING1_IMG = L"./textures/world/sprites/hex/Hex1.png";
	//wstring BOT_FLOATING2_IMG = L"./textures/world/sprites/hex/Hex2.png";
	//wstring BOT_FLOATING3_IMG = L"./textures/world/sprites/hex/Hex3.png";
	//int BOT_WIDTH = 64;
	//int BOT_HEIGHT = 64;
	//wstring FLOATING_STATE = L"FLOATING_STATE";

	//AnimatedSpriteType *ast = new AnimatedSpriteType();
	//vector<unsigned int> botImageIDs;
	//botImageIDs.push_back(worldTextureManager->loadTexture(BOT_FLOATING0_IMG));
	//botImageIDs.push_back(worldTextureManager->loadTexture(BOT_FLOATING1_IMG));
	//botImageIDs.push_back(worldTextureManager->loadTexture(BOT_FLOATING2_IMG));
	//botImageIDs.push_back(worldTextureManager->loadTexture(BOT_FLOATING3_IMG));
	//ast->setTextureSize(BOT_WIDTH, BOT_HEIGHT);
	//ast->addAnimationSequence(FLOATING_STATE);
	//for (int i = 0; i < 4; i++)
	//	ast->addAnimationFrame(FLOATING_STATE, botImageIDs.at(i), 10);
	//unsigned int spriteTypeID = spriteManager->addSpriteType(ast);
	//ast->setSpriteTypeID(spriteTypeID);

	//SimplePatternBot *bot = new SimplePatternBot(gsm->getPhysics());
	//	bot->setSpriteType(ast);
	//	bot->setCurrentState(FLOATING_STATE);
	//	bot->setAlpha(255);
	//	PhysicalProperties *pp = bot->getPhysicalProperties();
	//	pp->setCollidable(false);
	//	int x = 400;
	//	int y = 1600;
	//	pp->setX(x);
	//	pp->setY(y);
	//	pp->setAccelerationX(0.0f);
	//	pp->setAccelerationY(0.0f);
	//	spriteManager->addBot(bot);
	//
}


void SoSDataLoader::loadPlayer(Game *game, wstring playerInitFile)
{
	GameStateManager *gsm = game->getGSM();
	GameGraphics *graphics = game->getGraphics();
	TextureManager *worldTextureManager = graphics->getWorldTextureManager();

	map<wstring, int> *playerMap = new map<wstring, int>();//Sprite identifier, spriteImageID
	AnimatedSpriteType *ast = new AnimatedSpriteType();


	BufferedTextFileReader reader;
	reader.initFile(playerInitFile);
	wstring line;
	unsigned int delimiterIndex;

	int numSprites = _wtoi(reader.getNextLine().c_str());
	wchar_t delim = ',';
	
	for(int i = 0; i < numSprites; i++)
	{
		line = reader.getNextLine();
		delimiterIndex = line.find(delim);
		wstring name = line.substr(0, delimiterIndex);
		wstring path = line.substr(delimiterIndex+1);
		(*playerMap)[name] = worldTextureManager->loadTexture(path);
	}

	line = reader.getNextLine();
	delimiterIndex = line.find(delim);
	int width = _wtoi(line.substr(0, delimiterIndex).c_str());
	int height = _wtoi(line.substr(delimiterIndex+1).c_str());

	

	ast->setTextureSize(height, width);
	

	int numStates = _wtoi(reader.getNextLine().c_str());
	
	//loads each state
	for(int i = 0; i < numStates; i++)
	{
		line = reader.getNextLine();
		delimiterIndex = line.find(delim);
		wstring stateName = line.substr(0, delimiterIndex);
		int numFrames = _wtoi(line.substr(delimiterIndex+1).c_str());
		ast->addAnimationSequence(stateName);

		//loads each frame in state
		for(int j = 0; j < numFrames; j++)
		{
			line = reader.getNextLine();
			delimiterIndex = line.find(delim);
			wstring sprite = line.substr(0, delimiterIndex);
			int duration = _wtoi(line.substr(delimiterIndex+1).c_str());
			ast->addAnimationFrame(stateName, (*playerMap)[sprite], duration);
		}
	}

	SpriteManager *spriteManager = gsm->getSpriteManager();
	unsigned int spriteTypeID = spriteManager->addSpriteType(ast);
	ast->setSpriteTypeID(spriteTypeID);

	AnimatedSprite *player = spriteManager->getPlayer();
	player->setSpriteType(ast);
	player->setIsStatic(false);
	player->setMobile(true);
	player->setCurrentlyCollidable(true);

	line = reader.getNextLine();
	delimiterIndex = line.find(delim);
	int bVWidth = _wtoi(line.substr(0, delimiterIndex).c_str());
	int bVHeight = _wtoi(line.substr(delimiterIndex+1).c_str());

	BoundingVolume *bv = player->getBoundingVolume();
	bv->setWidth(bVWidth);
	bv->setHeight(bVHeight);
	bv->setX(width/2);
	bv->setY(height/2);
	bv->setType(1);

	PhysicalProperties *playerProps = player->getPhysicalProperties();
	line = reader.getNextLine();
	delimiterIndex = line.find(delim);
	int x = _wtoi(line.substr(0, delimiterIndex).c_str());
	int y = _wtoi(line.substr(delimiterIndex+1).c_str());
	playerProps->setX(x);
	playerProps->setY(y);
	
	playerProps->setCollidable(true);
	playerProps->setGravAffected(true);
	playerProps->setMass(1);

	playerProps->setVelocity(0.0f, 0.0f);
	playerProps->setAccelerationX(0);
	playerProps->setAccelerationY(0);
	
	player->setAlpha(_wtoi(reader.getNextLine().c_str()));
	player->setCurrentState(reader.getNextLine());

	delete playerMap;
}

void SoSDataLoader::loadBots(Game *game, wstring botInitFile)
{
	GameStateManager *gsm = game->getGSM();
	GameRules* gR = game->getGameRules();
	GameGraphics *graphics = game->getGraphics();
	TextureManager *worldTextureManager = graphics->getWorldTextureManager();
	SpriteManager *spriteManager = gsm->getSpriteManager();

	map<wstring, int> *botMap = new map<wstring, int>();//Sprite identifier, spriteImageID
	

	BufferedTextFileReader botLocReader;
	botLocReader.initFile(botInitFile);
	wstring line;
	unsigned int delimiterIndex;
	unsigned int typeCounter = 0;

	

	while(botLocReader.hasMoreLines())
	{
		line = botLocReader.getNextLine();
		AnimatedSpriteType *ast = new AnimatedSpriteType();
		BufferedTextFileReader reader;
		reader.initFile(line);
		
		int numSprites = _wtoi(reader.getNextLine().c_str());
		wchar_t delim = ',';

		for(int i = 0; i < numSprites; i++)
		{
			line = reader.getNextLine();
			delimiterIndex = line.find(delim);
			wstring name = line.substr(0, delimiterIndex);
			wstring path = line.substr(delimiterIndex+1);
			(*botMap)[name] = worldTextureManager->loadTexture(path);
		}

		line = reader.getNextLine();
		delimiterIndex = line.find(delim);
		int width = _wtoi(line.substr(0, delimiterIndex).c_str());
		int height = _wtoi(line.substr(delimiterIndex+1).c_str());

		ast->setTextureSize(height, width);
	
		//line = reader.getNextLine();
		int numStates = _wtoi(reader.getNextLine().c_str());
	
		//loads each state
		for(int i = 0; i < numStates; i++)
		{
			line = reader.getNextLine();
			delimiterIndex = line.find(delim);
			wstring stateName = line.substr(0, delimiterIndex);
			int numFrames = _wtoi(line.substr(delimiterIndex+1).c_str());
			ast->addAnimationSequence(stateName);

			//loads each frame in state
			for(int j = 0; j < numFrames; j++)
			{
				line = reader.getNextLine();
				delimiterIndex = line.find(delim);
				wstring sprite = line.substr(0, delimiterIndex);
				int duration = _wtoi(line.substr(delimiterIndex+1).c_str());
				ast->addAnimationFrame(stateName, (*botMap)[sprite], duration);
			}//end for
		}//end for

		unsigned int spriteTypeID = spriteManager->addSpriteType(ast);
		ast->setSpriteTypeID(spriteTypeID);

		gR->setBotSprite(ast, typeCounter);

		typeCounter ++;
		botMap->clear();
	}
	delete botMap;
}


/*
	initSoSGUI - This method builds a GUI for the SoS Game application.
	Note that we load all the GUI components from this method, including
	the ScreenGUI with Buttons and Overlays and the Cursor.
*/
void SoSDataLoader::hardCodedLoadGUIExample(Game *game)
{
	GameGUI *gui = game->getGUI();
	GameGraphics *graphics = game->getGraphics();
	DirectXTextureManager *guiTextureManager = (DirectXTextureManager*)graphics->getGUITextureManager();

	// SETUP THE CURSOR VIA OUR HELPER METHOD
	initCursor(gui, guiTextureManager);
	initSplashScreen(game, gui, guiTextureManager);
	initMainMenu(gui, guiTextureManager);
	initInGameGUI(gui, guiTextureManager);
}

/*
	initCursor - initializes a simple little cursor for the gui.
*/
void SoSDataLoader::initCursor(GameGUI *gui, DirectXTextureManager *guiTextureManager)
{
	// SETUP THE CURSOR
	vector<unsigned int> *imageIDs = new vector<unsigned int>();
	int imageID;

	// - FIRST LOAD THE GREEN CURSOR IMAGE
	imageID = guiTextureManager->loadTexture(SoS_GREEN_CURSOR_PATH);
	imageIDs->push_back(imageID);

	// - AND NOW THE RED ONE
	imageID = guiTextureManager->loadTexture(SoS_RED_CURSOR_PATH);
	imageIDs->push_back(imageID);

	// - NOW BUILD AND LOAD THE CURSOR
	Cursor *cursor = new Cursor();
	cursor->initCursor(	imageIDs,
						*(imageIDs->begin()),
						0,
						0,
						0,
						255,
						32,
						32);
	gui->setCursor(cursor);
}

/*
	initSplashScreen - initializes the game's splash screen gui.
*/
void SoSDataLoader::initSplashScreen(Game *game, GameGUI *gui,	DirectXTextureManager *guiTextureManager)
{
	// NOW, FIRST LET'S ADD A SPLASH SCREEN GUI
	ScreenGUI *splashScreenGUI = new ScreenGUI();

	// WE'LL ONLY HAVE ONE IMAGE FOR OUR GIANT BUTTON
	unsigned int normalTextureID = guiTextureManager->loadTexture(SoS_SPLASH_SCREEN_PATH);
	unsigned int mouseOverTextureID = normalTextureID;

	// INIT THE QUIT BUTTON
	Button *buttonToAdd = new Button();
	buttonToAdd->initButton(normalTextureID, 
							mouseOverTextureID,
							0,
							0,
							0,
							255,
							game->getGraphics()->getScreenWidth(),
							game->getGraphics()->getScreenHeight(),
							false,
							SoS_GO_TO_MM_COMMAND);
	splashScreenGUI->addButton(buttonToAdd);

	// AND REGISTER IT WITH THE GUI
	gui->addScreenGUI(GS_SPLASH_SCREEN, splashScreenGUI);
}

/*
	initMainMenu - initializes the game's main menu gui.
*/
void SoSDataLoader::initMainMenu(GameGUI *gui,	DirectXTextureManager *guiTextureManager)
{
	// NOW LET'S LOAD A MAIN MENU GUI SCREEN
	ScreenGUI *mainMenuGUI = new ScreenGUI();
	unsigned int imageID = guiTextureManager->loadTexture(SoS_MAIN_MENU_PATH);
	OverlayImage *imageToAdd = new OverlayImage();
	imageToAdd->x = 256;
	imageToAdd->y = 100;
	imageToAdd->z = 0;
	imageToAdd->alpha = 200;
	imageToAdd->width = 512;
	imageToAdd->height = 512;
	imageToAdd->imageID = imageID;
	mainMenuGUI->addOverlayImage(imageToAdd);

	// AND LET'S ADD AN EXIT BUTTON
	Button *buttonToAdd = new Button();

	// - GET THE BUTTON COMMAND AND IMAGE IDs
	int normalTextureID = guiTextureManager->loadTexture(SoS_EXIT_IMAGE_PATH);
	int mouseOverTextureID = guiTextureManager->loadTexture(SoS_EXIT_IMAGE_MO_PATH);

	// - INIT THE EXIT BUTTON
	buttonToAdd->initButton(normalTextureID, 
							mouseOverTextureID,
							412,
							500,
							0,
							255,
							200,
							100,
							false,
							SoS_EXIT_COMMAND);

	// AND NOW LOAD IT INTO A ScreenGUI
	mainMenuGUI->addButton(buttonToAdd);

	// AND LET'S ADD A START BUTTON
	buttonToAdd = new Button();

	// - GET THE BUTTON COMMAND AND IMAGE IDs

	normalTextureID = guiTextureManager->loadTexture(SoS_START_IMAGE_PATH);
	mouseOverTextureID = guiTextureManager->loadTexture(SoS_START_IMAGE_MO_PATH);

	// - INIT THE START BUTTON
	buttonToAdd->initButton(normalTextureID, 
							mouseOverTextureID,
							412,
							350,
							0,
							255,
							200,
							100,
							false,
							SoS_START_COMMAND);

	// AND NOW LOAD IT INTO A ScreenGUI
	mainMenuGUI->addButton(buttonToAdd);

	// AND LET'S ADD OUR SCREENS
	gui->addScreenGUI(GS_MAIN_MENU,mainMenuGUI);
}

/*
	initInGameGUI - initializes the game's in-game gui.
*/
void SoSDataLoader::initInGameGUI(GameGUI *gui, DirectXTextureManager *guiTextureManager)
{
	// NOW ADD THE IN-GAME GUI
	ScreenGUI *inGameGUI = new ScreenGUI();

	unsigned int normalTextureID = guiTextureManager->loadTexture(SoS_QUIT_IMAGE_PATH);
	unsigned int mouseOverTextureID = guiTextureManager->loadTexture(SoS_QUIT_IMAGE_MO_PATH);

	// INIT THE QUIT BUTTON
	Button *buttonToAdd = new Button();
	buttonToAdd->initButton(normalTextureID, 
							mouseOverTextureID,
							0,
							0,
							0,
							255,
							200,
							100,
							false,
							SoS_QUIT_COMMAND);
	inGameGUI->addButton(buttonToAdd);

	// AND LET'S ADD OUR SCREENS
	gui->addScreenGUI(GS_GAME_IN_PROGRESS,	inGameGUI);
	gui->addScreenGUI(GS_PAUSED, inGameGUI);
}

/*
	initViewport - initializes the game's viewport.
*/
void SoSDataLoader::initViewport(GameGUI *gui, map<wstring,wstring> *properties)
{
	// AND NOW SPECIFY THE VIEWPORT SIZE AND LOCATION. NOTE THAT IN THIS EXAMPLE,
	// WE ARE PUTTING A TOOLBAR WITH A BUTTON ACCROSS THE NORTH OF THE APPLICATION.
	// THAT TOOLBAR HAS A HEIGHT OF 64 PIXELS, SO WE'LL MAKE THAT THE OFFSET FOR
	// THE VIEWPORT IN THE Y AXIS
	Viewport *viewport = gui->getViewport();

	int viewportOffsetX, viewportOffsetY, screenWidth, screenHeight;
	wstring viewportOffsetXProp = (*properties)[SoS_VIEWPORT_OFFSET_X];
	wstring viewportOffsetYProp = (*properties)[SoS_VIEWPORT_OFFSET_Y];
	wstring screenWidthProp = (*properties)[SoS_SCREEN_WIDTH];
	wstring screenHeightProp = (*properties)[SoS_SCREEN_HEIGHT];
	wstringstream(viewportOffsetXProp) >> viewportOffsetX;
	wstringstream(viewportOffsetYProp) >> viewportOffsetY;
	wstringstream(screenWidthProp) >> screenWidth;
	wstringstream(screenHeightProp) >> screenHeight;
	int viewportWidth = screenWidth - viewportOffsetX;
	int viewportHeight = screenHeight - viewportOffsetY;
	viewport->setViewportWidth(viewportWidth);
	viewport->setViewportHeight(viewportHeight);
	viewport->setViewportOffsetX(viewportOffsetX);
	viewport->setViewportOffsetY(viewportOffsetY);
}

/*
	loadLevelExample - This method loads an example level via a hard-coded
	approach. This can be useful for testing code, but is not a practical
	solution. Game levels should be loaded from files.
*/
void SoSDataLoader::hardCodedLoadLevelExample(Game *game)
{
	// FIRST SETUP THE GAME WORLD DIMENSIONS
	GameStateManager *gsm = game->getGSM();
	GameGraphics *graphics = game->getGraphics();
	TextureManager *worldTextureManager = graphics->getWorldTextureManager();

	// NOTE THAT THE WORLD WILL BE THE SAME DIMENSIONS AS OUR TILED LAYER,
	// WE COULD GET A PARALLAX SCROLLING EFFECT IF WE MADE A LAYER SMALLER,
	// AND THEN SCROLLED IT SLOWER
	World *world = gsm->getWorld();
	world->setWorldWidth(NUM_COLUMNS * TILE_WIDTH);
	world->setWorldHeight(NUM_ROWS * TILE_HEIGHT);

	// NOW LOAD OUR TILED BACKGROUND
	TiledLayer *tiledLayer = new TiledLayer(	NUM_COLUMNS,	NUM_ROWS, 
												TILE_WIDTH,		TILE_HEIGHT, 
												0, true, 
												NUM_COLUMNS * TILE_WIDTH,
												NUM_ROWS * TILE_HEIGHT);
	int grassID = worldTextureManager->loadTexture(GRASS_IMAGE_PATH);
	int wallID = worldTextureManager->loadTexture(WALL_IMAGE_PATH);
	srand(1);

	// LET'S GENERATE A RANDOM BACKGROUND USING OUR TWO TILES
	for (int i = 0; i < (NUM_COLUMNS * NUM_ROWS); i++)
	{
		bool isCollidable = false;
		int tileIDToUse = grassID;
		int randomInt = rand() % 100;
		if (randomInt >= 50)
			isCollidable = true;

		randomInt = rand() % 100;
		if (randomInt >= 80)
			tileIDToUse = wallID;

		Tile *tileToAdd = new Tile();
		tileToAdd->collidable = isCollidable;
		tileToAdd->textureID = tileIDToUse;
		tiledLayer->addTile(tileToAdd);
	}

	// BUT LET'S CLEAR OUT THE TOP-LEFT AREA
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			Tile *tile = tiledLayer->getTile(i,j);
			tile->textureID = grassID;
		}
	}	

	world->addLayer(tiledLayer);

	loadPlayer(game, L"data/PlayerInfo.csv");
	loadBots(game, L"data/botInfo.csv");
	// AND NOW LET'S MAKE A MAIN CHARACTER SPRITE
	/*AnimatedSpriteType *ast = new AnimatedSpriteType();
	int spriteImageID0 = worldTextureManager->loadTexture(PLAYER_IDLE0_PATH);
	int spriteImageID1 = worldTextureManager->loadTexture(PLAYER_IDLE1_PATH);
	int spriteImageID2 = worldTextureManager->loadTexture(PLAYER_IDLE2_PATH);
	int spriteImageID3 = worldTextureManager->loadTexture(PLAYER_IDLE3_PATH);
	int spriteImageIDR0 = worldTextureManager->loadTexture(PLAYER_RIGHT0_PATH);
	int spriteImageIDR1 = worldTextureManager->loadTexture(PLAYER_RIGHT1_PATH);
	int spriteImageIDR2 = worldTextureManager->loadTexture(PLAYER_RIGHT2_PATH);
	int spriteImageIDR3 = worldTextureManager->loadTexture(PLAYER_RIGHT3_PATH);
	int spriteImageIDR4 = worldTextureManager->loadTexture(PLAYER_RIGHT4_PATH);
	int spriteImageIDR5 = worldTextureManager->loadTexture(PLAYER_RIGHT5_PATH);
	int spriteImageIDR6 = worldTextureManager->loadTexture(PLAYER_RIGHT6_PATH);
	int spriteImageIDR7 = worldTextureManager->loadTexture(PLAYER_RIGHT7_PATH);
	int spriteImageIDL0 = worldTextureManager->loadTexture(PLAYER_LEFT0_PATH);
	int spriteImageIDL1 = worldTextureManager->loadTexture(PLAYER_LEFT1_PATH);
	int spriteImageIDL2 = worldTextureManager->loadTexture(PLAYER_LEFT2_PATH);
	int spriteImageIDL3 = worldTextureManager->loadTexture(PLAYER_LEFT3_PATH);
	int spriteImageIDL4 = worldTextureManager->loadTexture(PLAYER_LEFT4_PATH);
	int spriteImageIDL5 = worldTextureManager->loadTexture(PLAYER_LEFT5_PATH);
	int spriteImageIDL6 = worldTextureManager->loadTexture(PLAYER_LEFT6_PATH);
	int spriteImageIDL7 = worldTextureManager->loadTexture(PLAYER_LEFT7_PATH);

	// SIZE OF SPRITE IMAGES
	ast->setTextureSize(PLAYER_WIDTH, PLAYER_HEIGHT);

	// NOW LET'S ADD AN ANIMATION STATE
	// FIRST THE NAME
	ast->addAnimationSequence(IDLE_STATE);
	ast->addAnimationFrame(IDLE_STATE, spriteImageID0, 50);
	ast->addAnimationFrame(IDLE_STATE, spriteImageID1, 5);
	ast->addAnimationFrame(IDLE_STATE, spriteImageID2, 5);
	ast->addAnimationFrame(IDLE_STATE, spriteImageID3, 50);
	ast->addAnimationFrame(IDLE_STATE, spriteImageID2, 5);
	ast->addAnimationFrame(IDLE_STATE, spriteImageID1, 5);
	ast->addAnimationFrame(IDLE_STATE, spriteImageID0, 20);

	ast->addAnimationSequence(L"RIGHT_STATE");
	ast->addAnimationFrame(L"RIGHT_STATE", spriteImageIDR0, 5);
	ast->addAnimationFrame(L"RIGHT_STATE", spriteImageIDR1, 5);
	ast->addAnimationFrame(L"RIGHT_STATE", spriteImageIDR2, 5);
	ast->addAnimationFrame(L"RIGHT_STATE", spriteImageIDR3, 5);
	ast->addAnimationFrame(L"RIGHT_STATE", spriteImageIDR4, 5);
	ast->addAnimationFrame(L"RIGHT_STATE", spriteImageIDR5, 5);
	ast->addAnimationFrame(L"RIGHT_STATE", spriteImageIDR6, 5);
	ast->addAnimationFrame(L"RIGHT_STATE", spriteImageIDR7, 5);

	ast->addAnimationSequence(L"LEFT_STATE");
	ast->addAnimationFrame(L"LEFT_STATE", spriteImageIDL0, 5);
	ast->addAnimationFrame(L"LEFT_STATE", spriteImageIDL1, 5);
	ast->addAnimationFrame(L"LEFT_STATE", spriteImageIDL2, 5);
	ast->addAnimationFrame(L"LEFT_STATE", spriteImageIDL3, 5);
	ast->addAnimationFrame(L"LEFT_STATE", spriteImageIDL4, 5);
	ast->addAnimationFrame(L"LEFT_STATE", spriteImageIDL5, 5);
	ast->addAnimationFrame(L"LEFT_STATE", spriteImageIDL6, 5);
	ast->addAnimationFrame(L"LEFT_STATE", spriteImageIDL7, 5);

	SpriteManager *spriteManager = gsm->getSpriteManager();
	unsigned int spriteTypeID = spriteManager->addSpriteType(ast);
	ast->setSpriteTypeID(spriteTypeID);

	AnimatedSprite *player = spriteManager->getPlayer();
	player->setSpriteType(ast);
	PhysicalProperties *playerProps = player->getPhysicalProperties();
	playerProps->setX(10);
	playerProps->setY(10);
	playerProps->setVelocity(0.0f, 0.0f);
	playerProps->setAccelerationX(0);
	playerProps->setAccelerationY(0);

	// WE WILL SET LOTS OF OTHER PROPERTIES ONCE
	// WE START DOING COLLISIONS AND PHYSICS

	player->setAlpha(255);
	player->setCurrentState(IDLE_STATE);*/
	SpriteManager *spriteManager = gsm->getSpriteManager();


	wstring BOT_FLOATING0_IMG = L"./textures/world/sprites/hex/Hex0.png";
	wstring BOT_FLOATING1_IMG = L"./textures/world/sprites/hex/Hex1.png";
	wstring BOT_FLOATING2_IMG = L"./textures/world/sprites/hex/Hex2.png";
	wstring BOT_FLOATING3_IMG = L"./textures/world/sprites/hex/Hex3.png";
	int BOT_WIDTH = 64;
	int BOT_HEIGHT = 64;
	wstring FLOATING_STATE = L"FLOATING_STATE";

	AnimatedSpriteType *ast = new AnimatedSpriteType();
	vector<unsigned int> botImageIDs;
	botImageIDs.push_back(worldTextureManager->loadTexture(BOT_FLOATING0_IMG));
	botImageIDs.push_back(worldTextureManager->loadTexture(BOT_FLOATING1_IMG));
	botImageIDs.push_back(worldTextureManager->loadTexture(BOT_FLOATING2_IMG));
	botImageIDs.push_back(worldTextureManager->loadTexture(BOT_FLOATING3_IMG));
	ast->setTextureSize(BOT_WIDTH, BOT_HEIGHT);
	ast->addAnimationSequence(FLOATING_STATE);
	for (int i = 0; i < 4; i++)
		ast->addAnimationFrame(FLOATING_STATE, botImageIDs.at(i), 10);
	unsigned int spriteTypeID = spriteManager->addSpriteType(ast);
	ast->setSpriteTypeID(spriteTypeID);

	SimplePatternBot *bot = new SimplePatternBot(gsm->getPhysics());
		bot->setSpriteType(ast);
		bot->setCurrentState(FLOATING_STATE);
		bot->setAlpha(255);
		PhysicalProperties *pp = bot->getPhysicalProperties();
		pp->setCollidable(false);
		int x = 400;
		int y = 400;
		pp->setX(x);
		pp->setY(y);
		pp->setAccelerationX(0.0f);
		pp->setAccelerationY(0.0f);
		spriteManager->addBot(bot);
}

GameState SoSDataLoader::gsLookup(wstring g)
{
	if(g.compare(L"GS_SPLASH_SCREEN") == 0)
	{
		return GS_SPLASH_SCREEN;
	}
	if(g.compare(L"GS_MAIN_MENU")== 0)
	{
		return GS_MAIN_MENU;
	}
	if(g.compare(L"GS_STARTING_LEVEL")== 0)
	{
		return GS_STARTING_LEVEL;
	}
	if(g.compare(L"GS_GAME_IN_PROGRESS")== 0)
	{
		return GS_GAME_IN_PROGRESS;
	}
	if(g.compare(L"GS_EXIT_GAME")== 0)
	{
		return GS_EXIT_GAME;
	}
	if(g.compare(L"GS_MENU_HELP_MENU")== 0)
	{
		return GS_MENU_HELP_MENU;
	}
	if(g.compare(L"GS_MENU_ABOUT_MENU")== 0)
	{
		return GS_MENU_ABOUT_MENU;
	}
	if(g.compare(L"GS_MENU_CONTROLS_MENU")== 0)
	{
		return GS_MENU_CONTROLS_MENU;
	}
	if(g.compare(L"GS_PAUSED")== 0)
	{
		return GS_PAUSED;
	}
	if(g.compare(L"GS_GAME_OVER")== 0)
	{
		return GS_GAME_OVER;
	}
	if(g.compare(L"GS_WIN_SCREEN")== 0)
	{
		return GS_WIN_SCREEN;
	}
	if(g.compare(L"GS_IN_GAME_HELP")== 0)
	{
		return GS_IN_GAME_HELP;
	}
	if(g.compare(L"GS_IN_GAME_ABOUT")== 0)
	{
		return GS_IN_GAME_ABOUT;
	}
	if(g.compare(L"GS_IN_GAME_CONTROLS")== 0)
	{
		return GS_IN_GAME_CONTROLS;
	}
	if(g.compare(L"GS_UNLOADING_LEVEL")== 0)
	{
		return GS_UNLOADING_LEVEL;
	}
	if(g.compare(L"GS_UNLOADING_GAME")== 0)
	{
		return GS_UNLOADING_GAME;
	}

	return GS_EXIT_GAME;
}