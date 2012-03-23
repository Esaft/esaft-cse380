#include "stdafx.h"
#include "SSSF_SourceCode\game\Game.h"
#include "SSSF_SourceCode\gsm\ai\SimpleTrackingBot.h"
#include "SSSF_SourceCode\gsm\state\GameStateManager.h"
#include "SSSF_SourceCode\gsm\physics\Physics.h"
#include "SSSF_SourceCode\gsm\physics\PhysicalProperties.h"



Bot* SimpleTrackingBot::clone()
{
	SimpleTrackingBot *botClone = new SimpleTrackingBot(velMag, trackX, trackY);
	return botClone;
}

SimpleTrackingBot::SimpleTrackingBot(float vM, bool tX, bool tY)
{
	velMag = vM;
	trackX = tX;
	trackY = tY;
	enemy = true;
	dead = false;
}

void SimpleTrackingBot::think(Game *game)
{
	
	PhysicalProperties* pp	= &(this->pp);
	if(dead)
	{
		PhysicalProperties* playerProps = game->getGSM()->getSpriteManager()->getPlayer()->getPhysicalProperties();
	

		if (trackX == true)
		{
			if(playerProps->getX() < pp->getX())
			{
				if(pp->getVelocityX() >= 0)
				{
					
					this->setCurrentState(L"LEFT_STATE");
					pp->setVelocity(-velMag,pp->getVelocityY());
					pp->setOrientedLeft();
				}
			}
			else
			{	
				if(pp->getVelocityX() < 0)
				{
					this->setCurrentState(L"RIGHT_STATE");
					pp->setVelocity(velMag,pp->getVelocityY());
					pp->setOrientedRight();
				}
			}
		}
		if (trackY == true)
		{
			if(playerProps->getY() < pp->getY())
				pp->setVelocity(pp->getVelocityX(),-velMag);
			else
				pp->setVelocity(pp->getVelocityX(),velMag);
		}
	}
	else
	{
		wstring curState = this->getCurrentState();
		pp->setVelocity(0,pp->getVelocityY());
		int lastFrame = this->getSpriteType()->getSequenceSize(curState)-2;
		if(this->getFrameIndex()/2 == lastFrame)
		{
			game->getGSM()->getSpriteManager()->removeBot(this);
		}
	}
}

void SimpleTrackingBot::setDead(bool d)
{
	dead = d;

	PhysicalProperties* pp	= &(this->pp);
	if(dead)
	{
		this->setCurrentState(L"DIE_STATE");
		if(!pp->isOrientedRight())
		{
			this->setCurrentState(L"DIEL_STATE");
		}
	}
}