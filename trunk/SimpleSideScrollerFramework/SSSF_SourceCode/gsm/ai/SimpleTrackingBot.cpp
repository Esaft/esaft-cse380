#include "stdafx.h"
#include "SSSF_SourceCode\game\Game.h"
#include "SSSF_SourceCode\gsm\ai\SimpleTrackingBot.h"
#include "SSSF_SourceCode\gsm\state\GameStateManager.h"
#include "SSSF_SourceCode\gsm\physics\Physics.h"
#include "SSSF_SourceCode\gsm\physics\PhysicalProperties.h"



Bot* SimpleTrackingBot::clone()
{
	SimpleTrackingBot *botClone = new SimpleTrackingBot(velMag);
	return botClone;
}

SimpleTrackingBot::SimpleTrackingBot(float vM)
{
	velMag = vM;
}

void SimpleTrackingBot::think(Game *game)
{
	PhysicalProperties* playerProps = game->getGSM()->getSpriteManager()->getPlayer()->getPhysicalProperties();
	PhysicalProperties* pp	= &(this->pp);

	if (trackX == true)
	{
		if(playerProps->getX() < pp->getX())
			pp->setVelocity(-velMag,pp->getVelocityY());
		else
			pp->setVelocity(velMag,pp->getVelocityY());
	}
	if (trackY == true)
	{
		if(playerProps->getY() < pp->getY())
			pp->setVelocity(pp->getVelocityX(),-velMag);
		else
			pp->setVelocity(pp->getVelocityX(),velMag);
	}
}