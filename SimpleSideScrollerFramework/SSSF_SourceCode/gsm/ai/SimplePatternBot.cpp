#include "stdafx.h"
#include "SSSF_SourceCode\game\Game.h"
#include "SSSF_SourceCode\gsm\ai\SimplePatternBot.h"
#include "SSSF_SourceCode\gsm\state\GameStateManager.h"
#include "SSSF_SourceCode\gsm\physics\Physics.h"



/*
	This private constructor is only to be used for cloning bots, note
	that this does not setup the velocity for this bot.
*/
SimplePatternBot::SimplePatternBot()
{
	pathCounter = 0;
	pathSize = sizeof(paths)/sizeof(float);
}

/*
	This is the public constructor used by other classes for 
	creating these types of bots.
*/
SimplePatternBot::SimplePatternBot(	Physics *physics)
{
	//initBot();
	pathCounter = 0;
	pathSize = sizeof(paths)/sizeof(float);
	pickNextVelocity(physics);
	cyclesRemainingBeforeChange = paths[pathCounter+2];
}

/*
	clone - this method makes another SimplePatternBot object, but does
	not completely initialize it with similar data to this. Most of the 
	object, like velocity and position, are left uninitialized.
*/
Bot* SimplePatternBot::clone()
{
	SimplePatternBot *botClone = new SimplePatternBot();
	return botClone;
}

/*


/*
	pickRandomVelocity - calculates a random velocity vector for this
	bot and initializes the appropriate instance variables.
*/
void SimplePatternBot::pickNextVelocity(Physics *physics)
{
	this->pp.setVelocitySafely(physics, paths[pathCounter], paths[pathCounter+1]);
}

/*
	think - called once per frame, this is where the bot performs its
	decision-making. Note that we might not actually do any thinking each
	frame, depending on the value of cyclesRemainingBeforeThinking.
*/
void SimplePatternBot::think(Game *game)
{
	// EACH FRAME WE'LL TEST THIS BOT TO SEE IF WE NEED
	// TO PICK A DIFFERENT DIRECTION TO FLOAT IN

	if (cyclesRemainingBeforeChange == 0)
	{
		pathCounter = (pathCounter + 3)% pathSize;	
		GameStateManager *gsm = game->getGSM();
		pickNextVelocity(gsm->getPhysics());
		cyclesRemainingBeforeChange = paths[pathCounter+2];
			
	}
	else
		cyclesRemainingBeforeChange--;
}