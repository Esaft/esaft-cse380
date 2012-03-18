#pragma once

// PREPROCESSOR INCLUDE STATEMENTS
#include "stdafx.h"
#include "SSSF_SourceCode\gsm\ai\Bot.h"
#include "SSSF_SourceCode\gsm\physics\Physics.h"
#include "SSSF_SourceCode\game\Game.h"


const float paths[]  =  {-5,0,10, 5,0,20, -5,-5,10, 0,0,10, 0,20,5, -10,0,5, 5,-5,10, 0,0,10 };//Stores sets of 3 numbers: path X velocity, path Y velocity, path duration

class SimplePatternBot : public Bot
{
private:
	unsigned int cyclesRemainingBeforeChange;
	unsigned int pathCounter;
	unsigned int pathSize;
	
	

	// THIS PRIVATE CONSTRUCTOR IS ONLY USED FOR CLONING
	SimplePatternBot();

public:
	SimplePatternBot(Physics *physics);
	~SimplePatternBot();
	Bot*	clone();
	//void initBot();
	void	pickNextVelocity(Physics *physics);
	void	think(Game *game);

	// INLINED METHODS
	unsigned int getCyclesRemainingBeforeChange()	{ return cyclesRemainingBeforeChange; }

};