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
}

void SimpleTrackingBot::think(Game *game)
{
	wstring curState = this->getCurrentState();
	if(curState.compare(L"DIE_STATE") != 0 && curState.compare(L"DIEL_STATE") != 0)
	{
		PhysicalProperties* playerProps = game->getGSM()->getSpriteManager()->getPlayer()->getPhysicalProperties();
		PhysicalProperties* pp	= &(this->pp);

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
		int lastFrame = this->getSpriteType()->getSequenceSize(curState)-2;
		if(this->getFrameIndex()/2 == lastFrame)
		{
			game->getGSM()->getSpriteManager()->removeBot(this);
		}
	}
}