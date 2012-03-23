#include "stdafx.h"
#include "SSSF_SourceCode\game\Game.h"
#include "SSSF_SourceCode\gsm\sprite\AnimatedSprite.h"
#include "SSSF_SourceCode\gsm\ai\Bot.h"
#include "SSSF_SourceCode\gsm\ai\ItemBot.h"
#include "SSSF_SourceCode\gsm\physics\PhysicalProperties.h"
#include "SSSF_SourceCode\gsm\physics\BoundingVolume.h"



Bot*	ItemBot::clone() 
	{ 
		

		ItemBot* botClone = new ItemBot();
		PhysicalProperties* pp = this->getPhysicalProperties();
		PhysicalProperties* bp = botClone->getPhysicalProperties();
		BoundingVolume* pV = this->getBoundingVolume();
		BoundingVolume* bV = botClone->getBoundingVolume();
		botClone->setItem(item);
		botClone->setPortal(portal);
		botClone->setSpriteType(this->getSpriteType());
		botClone->setAlpha(this->getAlpha());
		bp->setCollidable(pp->isCollidable());
		bp->setGravAffected(pp->isGravAffected());
		bV->setHeight(pV->getHeight());
		bV->setWidth(pV->getWidth());
		bV->setX(pV->getX());
		bV->setY(pV->getY());
		
		
		return botClone;
	}