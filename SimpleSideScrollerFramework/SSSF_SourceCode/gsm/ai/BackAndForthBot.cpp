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
	vel = 3.0f;
}

/*
	This is the public constructor used by other classes for 
	creating these types of bots.
*/
BackAndForthBot::BackAndForthBot(int duration)
{
	//initBot();
	vel = 2.0f;
	pathSize = duration;
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
	PhysicalProperties* pp = this->getPhysicalProperties();
	PhysicalProperties* bp = botClone->getPhysicalProperties();
	BoundingVolume* pV = this->getBoundingVolume();
	BoundingVolume* bV = botClone->getBoundingVolume();
	botClone->setEnemy(enemy);
	botClone->setItem(item);
	botClone->setPortal(portal);
	botClone->setSpriteType(this->getSpriteType());
	botClone->setAlpha(this->getAlpha());
	botClone->setDead(false);
	bp->setCollidable(pp->isCollidable());
	bp->setGravAffected(pp->isGravAffected());
	bV->setHeight(pV->getHeight());
	bV->setWidth(pV->getWidth());
	bV->setX(pV->getX());
	bV->setY(pV->getY());
	return botClone;
}

/*


/*
	think - called once per frame, this is where the bot performs its
	decision-making. Note that we might not actually do any thinking each
	frame, depending on the value of cyclesRemainingBeforeThinking.
*/
void BackAndForthBot::think(Game *game)
{
	// EACH FRAME WE'LL TEST THIS BOT TO SEE IF WE NEED
	// TO PICK A DIFFERENT DIRECTION TO FLOAT IN
	PhysicalProperties* pp	= &(this->pp);
	if(!dead)
	{
		if (cyclesRemainingBeforeChange == 0)
		{
			pp->setVelocity(-vel, pp->getVelocityY());

			if(pp->isOrientedRight())
			{
				this->setCurrentState(L"WALKL_STATE");
				pp->setOrientedLeft();
			}
			else
			{
				this->setCurrentState(L"WALK_STATE");
				pp->setOrientedRight();
			}

			cyclesRemainingBeforeChange = pathSize;

		}
		else
			cyclesRemainingBeforeChange--;

	}
	else
	{
		wstring curState = this->getCurrentState();
		pp->setVelocity(0,pp->getVelocityY());
		int lastFrame = this->getSpriteType()->getSequenceSize(curState)-2;
		if(this->getFrameIndex() == lastFrame)
		{
			game->getGSM()->getSpriteManager()->removeBot(this);
		}
	}
}