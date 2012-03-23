#pragma once
#include "stdafx.h"
#include "SoSGameRules.h"
#include "SSSF_SourceCode\game\Game.h"
#include "SSSF_SourceCode\gsm\state\GameStateManager.h"
#include "SSSF_SourceCode\gsm\ai\Bot.h"
#include "SSSF_SourceCode\gsm\sprite\AnimatedSpriteType.h"
#include "SSSF_SourceCode\gsm\ai\SimpleTrackingBot.h"
#include "SSSF_SourceCode\gsm\physics\PhysicalProperties.h"
#include "SSSF_SourceCode\gsm\physics\BoundingVolume.h"
#include "SSSF_SourceCode\gsm\physics\CollidableObject.h"



SoSGameRules::SoSGameRules()
{
	botTypeList = new vector<Bot*>();
	
	Bot* botToAdd;
	PhysicalProperties* pp;
	for(int i = 0; i < 1; i++)
	{
		if(i == 0)
		{
			botToAdd = new SimpleTrackingBot(1.0f,true,false);
		}
		if(i == 1);
		if(i == 2);
		if(i == 3);

		botToAdd->setIsStatic(false);
		pp = botToAdd->getPhysicalProperties();
		
		botTypeList->push_back(botToAdd);
	}
}

SoSGameRules::~SoSGameRules()
{
	delete botTypeList;
}

void SoSGameRules::setBotSprite(AnimatedSpriteType* ast, int enemyType)
{
	Bot* b = (botTypeList->at(enemyType));
	b->setSpriteType(ast);

	float width = ast->getTextureWidth();
	float height = ast->getTextureHeight();

	BoundingVolume *bv = b->getBoundingVolume();
	bv->setWidth(width);
	bv->setHeight(height);
	bv->setX(width/2);
	bv->setY(height/2);
	bv->setType(1);
}

Bot* SoSGameRules::getBot(int type)
{
	return botTypeList->at(type);
}

void SoSGameRules::gameSpecificResolve(Game* game, Collision* c)
{
	AnimatedSprite* player = game->getGSM()->getSpriteManager()->getPlayer();
	CollidableObject* other;

	if(c->getCO1() == player)
	{
		playerColResolve(player, c->getCO2());
	}
	else if(c->getCO2() == player)
	{
		playerColResolve(player, c->getCO1());
	}

	//Other sprite-sprite collisions really don't have any specific response in this game.

}

void SoSGameRules::playerColResolve(AnimatedSprite* player, CollidableObject* other)
{
	if(!other->isStaticObject())
	{
		Bot* b = static_cast<Bot*>(other);
		if(b->isEnemy())
		{
			PhysicalProperties* pp = b->getPhysicalProperties();

			if(player->getCurrentState().compare(L"ATTACK_STATE") == 0
				|| player->getCurrentState().compare(L"ATTACKl_STATE") == 0 )
			{
				if(pp->isOrientedRight())
				{
					b->setCurrentState(L"DIE_STATE");
				}
				else
				{
					b->setCurrentState(L"DIEL_STATE");
				}
			}
			else
			{
				
			}
		}
		else if(b->isItem())
		{
			if(b->getCurrentState().compare(L"CODE_STATE") == 0)
			{
				codeCollected ++;
			}
		}
		else if(b->isPortal())
		{

		}
	}
}

void SoSGameRules::spawnEnemies(Game* game)
{

}

void SoSGameRules::setUpGame(Game* game)
{
	health = 3;
	codeCollected = 0;
}