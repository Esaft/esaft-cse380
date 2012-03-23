#include "stdafx.h"
#include "SSSF_SourceCode\game\Game.h"
#include "SSSF_SourceCode\gsm\ai\BackAndForthBot.h"
#include "SSSF_SourceCode\gsm\state\GameStateManager.h"
#include "SSSF_SourceCode\gsm\physics\Physics.h"



/*
	This private constructor is only to be used for cloning bots, note
	that this does not setup the velocity for this bot.
*/
BackAndForthBot::BackAndForthBot()
{
	pathCounter = 0;
	
}

/*
	This is the public constructor used by other classes for 
	creating these types of bots.
*/
BackAndForthBot::BackAndForthBot(int duration)
{
	//initBot();
	pathCounter = 0;

}

/*
	clone - this method makes another BackAndForthBot object, but does
	not completely initialize it with similar data to this. Most of the 
	object, like velocity and position, are left uninitialized.
*/
Bot* BackAndForthBot::clone()
{
	BackAndForthBot *botClone = new BackAndForthBot();
	return botClone;
}

/*


/*
	pickRandomVelocity - calculates a random velocity vector for this
	bot and initializes the appropriate instance variables.
*/
void BackAndForthBot::pickNextVelocity(Physics *physics)
{
	
}

/*
	think - called once per frame, this is where the bot performs its
	decision-making. Note that we might not actually do any thinking each
	frame, depending on the value of cyclesRemainingBeforeThinking.
*/
void BackAndForthBot::think(Game *game)
{
	// EACH FRAME WE'LL TEST THIS BOT TO SEE IF WE NEED
	// TO PICK A DIFFERENT DIRECTION TO FLOAT IN

	if (cyclesRemainingBeforeChange == 0)
	{
			
	}
	else
		cyclesRemainingBeforeChange--;
}