#pragma once

// PREPROCESSOR INCLUDE STATEMENTS
#include "stdafx.h"
#include "SSSF_SourceCode\gsm\ai\Bot.h"
#include "SSSF_SourceCode\gsm\physics\Physics.h"
#include "SSSF_SourceCode\game\Game.h"

class BackAndForthBot : public Bot
{
private:
	unsigned int cyclesRemainingBeforeChange;
	unsigned int pathCounter;
	unsigned int pathSize;
	float vel;
	
	

	// THIS PRIVATE CONSTRUCTOR IS ONLY USED FOR CLONING
	BackAndForthBot();

public:
	BackAndForthBot(int duration);
	~BackAndForthBot();
	Bot*	clone();
	//void initBot();
	void	pickNextVelocity(Physics *physics);
	void	think(Game *game);

	// INLINED METHODS
	unsigned int getCyclesRemainingBeforeChange()	{ return cyclesRemainingBeforeChange; }

};