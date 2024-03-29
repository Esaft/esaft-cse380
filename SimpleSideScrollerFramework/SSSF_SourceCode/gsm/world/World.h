/*	
	Author: Richard McKenna
			Stony Brook University
			Computer Science Department

	World.h

	This class manages the static data for the game level being
	played. This means all game backgrounds, which are stored and
	manipulated in WorldLayer objects. A given level can have
	a game background rendered using multiple layers, some which
	can be tiled, others that can be sparse, and others that
	can be isometric.

	This class stores these layers and ensures they are rendered
	in the proper order, low index to high. Layers that need to
	be drawn first (the back-most layer), should be added first.
*/
#pragma once
#include "stdafx.h"
#include "SSSF_SourceCode\dataLoader\GameDataLoader.h"
#include "SSSF_SourceCode\graphics\RenderList.h"
#include "SSSF_SourceCode\gsm\world\WorldLayer.h"

class Game;
class SpriteManager;

class World
{
private:
	// NOTE, THE worldWidth & worldHeight REPRESENT THE FULL
	// SIZE OF THIS LEVEL. WHICH SHOULD BE AT LEAST THE SIZE OF
	// THE VIEWPORT, AND LIKELY MUCH LARGER, WHICH WOULD THEN
	// REQUIRE SCROLLING TO SEE IT.
	int worldWidth;
	int worldHeight;

	// THESE ARE THE BACKGROUND LAYERS
	vector<WorldLayer*> *layers;
	TiledLayer* tL;

public:
	// INLINED ACCESSOR METHODS
	vector<WorldLayer*>*	getLayers()	{ return layers;				}
	int						getWorldHeight()	{ return worldHeight;	}
	int						getWorldWidth()		{ return worldWidth;	}
	TiledLayer*				getTiledLayer()		{return tL;				}

	// INLINED MUTATOR METHODS
	void setWorldHeight(int initWorldHeight)
	{ worldHeight = initWorldHeight;		}
	void setWorldWidth(int initWorldWidth)
	{ worldWidth = initWorldWidth;			}
	void setTiledLayer(TiledLayer* t)	{tL = t;}

	// METHODS DEFINED in GameStateManager.cpp
	World();
	~World();
	void	addLayer(WorldLayer *layerToAdd);
	void	addWorldRenderItemsToRenderList(Game *game);
	void	unloadWorld();
	void	update(Game *game);
};