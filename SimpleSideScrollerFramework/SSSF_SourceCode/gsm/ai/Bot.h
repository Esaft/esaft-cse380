#pragma once

#include "stdafx.h"
#include "SSSF_SourceCode\game\Game.h"
#include "SSSF_SourceCode\gsm\sprite\AnimatedSprite.h"

class Bot : public AnimatedSprite
{
protected:

	bool dead;

public:
	Bot()	{ dead = false;}
	~Bot()	{}

	// TO BE DEFINED BY BOT AI CLASSES
	virtual void think(Game *game)=0;
	virtual Bot* clone()=0;

	
	bool isDead()	{return dead;}
	
	virtual void setDead(bool d) {dead = d;}
};