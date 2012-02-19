#pragma once
#include "stdafx.h"

/*
	DummyGame.h

	This file contains setup data for making the dummy game. Note that nearly
	all of this data would better serve us if loaded from files.
*/

// HERE IS THE DUMMY GAME SETUP/INIT FILE. EVERYTHING WOULD START FROM HERE
static const wstring	SoS_INIT_FILE			= L"data/SoSGameInit.txt";
static const wstring	SoS_GUI_INIT_FILE		= L"data/SoSGUIInit.txt";
static const string		SoS_DEBUG_FILE			= "DebugOutput.txt";


// HERE ARE THE GAME PROPERTIES WE ARE LOADING FROM FROM THE SETUP FILE
static const wstring	SoS_TITLE				= L"TITLE";
static const wstring	SoS_SCREEN_WIDTH			= L"SCREEN_WIDTH";
static const wstring	SoS_SCREEN_HEIGHT		= L"SCREEN_HEIGHT";
static const wstring	SoS_USE_FULLSCREEN_MODE	= L"USE_FULLSCREEN_MODE";
static const wstring	SoS_TEXT_FONT_SIZE		= L"TEXT_FONT_SIZE";
static const wstring	SoS_VIEWPORT_OFFSET_X	= L"VIEWPORT_OFFSET_X";
static const wstring	SoS_VIEWPORT_OFFSET_Y	= L"VIEWPORT_OFFSET_Y";
static const wstring	SoS_FONT_COLOR_RED		= L"FONT_COLOR_RED";
static const wstring	SoS_FONT_COLOR_GREEN		= L"FONT_COLOR_GREEN";
static const wstring	SoS_FONT_COLOR_BLUE		= L"FONT_COLOR_BLUE";
static const wstring	SoS_COLOR_KEY_RED		= L"COLOR_KEY_RED";
static const wstring	SoS_COLOR_KEY_GREEN		= L"COLOR_KEY_GREEN";
static const wstring	SoS_COLOR_KEY_BLUE		= L"COLOR_KEY_BLUE";


static const wstring	SoS_NUM_CURSORS			= L"NUM_CURSORS";
static const wstring	SoS_CURSOR_PATH			= L"CURSOR_PATH_";

//SCREEN PROPERTIES
static const wstring	SoS_NUM_SCREENS			= L"NUM_SCREENS";
static const wstring	SoS_SCREEN_NUM_IMAGES	= L"_SCREEN_NUM_IMAGES";
static const wstring	SoS_SCREEN_NUM_BUTTONS	= L"_SCREEN_NUM_BUTTONS";
static const wstring	SoS_SCREEN_NUM_STATES	= L"_SCREEN_NUM_STATES";
static const wstring	SoS_SCREEN_STATE		= L"_SCREEN_STATE_";

//IMAGE PROPERTIES
static const wstring	SoS_SCREEN_IMAGE_PATH	= L"_SCREEN_IMAGE_PATH_";
static const wstring	SoS_SCREEN_IMAGE_X		= L"_SCREEN_IMAGE_X_";
static const wstring	SoS_SCREEN_IMAGE_Y		= L"_SCREEN_IMAGE_Y_";
static const wstring	SoS_SCREEN_IMAGE_Z		= L"_SCREEN_IMAGE_Z_";
static const wstring	SoS_SCREEN_IMAGE_ALPHA	= L"_SCREEN_IMAGE_ALPHA_";
static const wstring	SoS_SCREEN_IMAGE_WIDTH	= L"_SCREEN_IMAGE_WIDTH_";
static const wstring	SoS_SCREEN_IMAGE_HEIGHT	= L"_SCREEN_IMAGE_HEIGHT_";

//BUTTON PROPERTIES
static const wstring	SoS_SCREEN_BUTTON_PATH	= L"_SCREEN_BUTTON_PATH_";
static const wstring	SoS_SCREEN_BUTTON_MO_PATH	= L"_SCREEN_BUTTON_MO_PATH_";
static const wstring	SoS_SCREEN_BUTTON_X		= L"_SCREEN_BUTTON_X_";
static const wstring	SoS_SCREEN_BUTTON_Y		= L"_SCREEN_BUTTON_Y_";
static const wstring	SoS_SCREEN_BUTTON_Z		= L"_SCREEN_BUTTON_Z_";
static const wstring	SoS_SCREEN_BUTTON_ALPHA	= L"_SCREEN_BUTTON_ALPHA_";
static const wstring	SoS_SCREEN_BUTTON_WIDTH	= L"_SCREEN_BUTTON_WIDTH_";
static const wstring	SoS_SCREEN_BUTTON_HEIGHT= L"_SCREEN_BUTTON_HEIGHT_";
static const wstring	SoS_SCREEN_BUTTON_MO	= L"_SCREEN_BUTTON_MO_";
static const wstring	SoS_SCREEN_BUTTON_COM	= L"_SCREEN_BUTTON_COM_";


// CONSTANTS FOR ARTWORK - NOTE, THIS SHOULD REALLY BE LOADED FROM A FILE
static const wstring SoS_GO_TO_MM_COMMAND	= L"Go To Main Menu";
static const wstring SoS_START_COMMAND		= L"Start";
static const wstring SoS_EXIT_COMMAND		= L"Exit";
static const wstring SoS_QUIT_COMMAND		= L"Quit";
static const wstring SoS_GREEN_CURSOR_PATH	= L"textures/gui/cursor/green_cursor.png";
static const wstring SoS_RED_CURSOR_PATH		= L"textures/gui/cursor/red_cursor.png";
static const wstring SoS_SPLASH_SCREEN_PATH	= L"textures/gui/overlays/sos_splash_screen_overlay.png";
static const wstring SoS_MAIN_MENU_PATH		= L"textures/gui/overlays/sos_main_menu_overlay.png";
static const wstring SoS_START_IMAGE_PATH	= L"textures/gui/buttons/start_game.png";
static const wstring SoS_START_IMAGE_MO_PATH	= L"textures/gui/buttons/start_game_mo.png";
static const wstring SoS_EXIT_IMAGE_PATH		= L"textures/gui/buttons/exit_game.png";
static const wstring SoS_EXIT_IMAGE_MO_PATH	= L"textures/gui/buttons/exit_game_mo.png";
static const wstring SoS_QUIT_IMAGE_PATH		= L"textures/gui/buttons/quit.png";
static const wstring SoS_QUIT_IMAGE_MO_PATH	= L"textures/gui/buttons/quit_mo.png";


// EXAMPLE LEVEL DATA FOR THE HARD-CODED loadLevelExample

// FOR BACKGROUND TILES
static const wstring	GRASS_IMAGE_PATH = L"textures/world/tiles/grass.png";
static const wstring	WALL_IMAGE_PATH = L"textures/world/tiles/wall.png";

// FOR OUR SPRITE
static const wstring	IDLE_STATE = L"IDLE_STATE";
static const wstring	PLAYER_IDLE0_PATH = L"textures/world/sprites/evan/evan_3_0.png";
static const wstring	PLAYER_IDLE1_PATH = L"textures/world/sprites/evan/evan_3_1.png";
static const wstring	PLAYER_IDLE2_PATH = L"textures/world/sprites/evan/evan_3_2.png";
static const wstring	PLAYER_IDLE3_PATH = L"textures/world/sprites/evan/evan_3_3.png";


static const int		ANIM_SPEED = 6;
static const int		PLAYER_WIDTH = 64;
static const int		PLAYER_HEIGHT = 128;

// FOR OUR WORLD
static const int		NUM_COLUMNS = 60;
static const int		NUM_ROWS = 40;
static const int		TILE_WIDTH = 64;
static const int		TILE_HEIGHT = 64;
static const int		Z_LAYER = 0;
