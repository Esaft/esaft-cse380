#pragma once

#include "stdafx.h"
#include "SSSF_SourceCode\game\Game.h"
#include "SSSF_SourceCode\gsm\sprite\AnimatedSprite.h"

class Bot : public AnimatedSprite
{
protected:
	bool enemy;
	bool item;
	bool portal;
	bool dead;

public:
	Bot()	{enemy = false; item = false; portal = false; dead = false;}
	~Bot()	{}

	// TO BE DEFINED BY BOT AI CLASSES
	virtual void think(Game *game)=0;
	virtual Bot* clone()=0;

	bool isEnemy()	{return enemy;}
	bool isItem()	{return item;}
	bool isPortal()	{return portal;}
	bool isDead()	{return dead;}
	void setEnemy()	{enemy = true;}
	void setItem()	{item = true;}
	void setPortal(){portal = true;}
	virtual void setDead(bool d) {dead = d;}
};