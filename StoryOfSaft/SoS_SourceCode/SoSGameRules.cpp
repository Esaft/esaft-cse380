#pragma once
#include "stdafx.h"
#include "SoSGameRules.h"
#include "SSSF_SourceCode\game\Game.h"
#include "SSSF_SourceCode\gsm\state\GameStateManager.h"
#include "SSSF_SourceCode\gsm\ai\Bot.h"
#include "SSSF_SourceCode\gsm\sprite\AnimatedSpriteType.h"
#include "SSSF_SourceCode\gsm\ai\SimpleTrackingBot.h"
#include "SSSF_SourceCode\gsm\ai\BackAndForthBot.h"
#include "SSSF_SourceCode\gsm\ai\ItemBot.h"
#include "SSSF_SourceCode\gsm\physics\PhysicalProperties.h"
#include "SSSF_SourceCode\gsm\physics\BoundingVolume.h"
#include "SSSF_SourceCode\gsm\physics\CollidableObject.h"



SoSGameRules::SoSGameRules()
{
	botTypeList = new vector<Bot*>();
	
	Bot* botToAdd;
	PhysicalProperties* pp;
	for(int i = 0; i < 4; i++)
	{
		if(i == 0)
		{
			botToAdd = new SimpleTrackingBot(1.0f,true,false);
		}
		if(i == 1)
		{
			botToAdd = new BackAndForthBot(200);
		}
		if(i == 2)
		{
			botToAdd = new ItemBot();
			botToAdd->setMobile(false);
		}
		if(i == 3)
		{
			botToAdd = new ItemBot();
			botToAdd->setItem(false);
			botToAdd->setPortal(true);
			botToAdd->setMobile(false);
		}

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
	bv->setWidth(width*0.6);
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
		playerColResolve(game, player, c->getCO2());
	}
	else if(c->getCO2() == player)
	{
		playerColResolve(game, player, c->getCO1());
	}

	/*if(health == 0)
	{
		game->quitGame();
		game->getGSM()->goToGameOver();
	}*/
	if(codeCollected == 1 && !doorPlaced)
	{
		placeDoor(game);
	}
	if(won == true)
	{
		game->quitGame();
		game->getGSM()->goToWinScreen();
	}

	//Other sprite-sprite collisions really don't have any specific response in this game.

}

void SoSGameRules::placeDoor(Game*game)
{

	Bot* bot = getBot(3);
	PhysicalProperties* pp = bot->getPhysicalProperties();
	pp->setX(doorX);
	pp->setY(doorY);
	bot->setAlpha(255);
	bot->setCurrentState(L"DOOR_STATE");
	bot->setCurrentlyCollidable(true);
	game->getGSM()->getSpriteManager()->addBot(bot);
}

void SoSGameRules::playerColResolve(Game* game, AnimatedSprite* player, CollidableObject* other)
{
	if(!other->isStaticObject())
	{
		Bot* b = static_cast<Bot*>(other);
		if(b->isEnemy())
		{
			
			PhysicalProperties* playerProps = player->getPhysicalProperties();
			PhysicalProperties* pp = b->getPhysicalProperties();
			

			if(player->getCurrentState().compare(L"ATTACK_STATE") == 0
           				|| player->getCurrentState().compare(L"ATTACKl_STATE") == 0 )
			{
				if(pp->isOrientedRight())
				{
					b->setCurrentlyCollidable(false);
					b->setDead(true);
				}
				else
				{
					b->setCurrentlyCollidable(false);
					b->setDead(true);
				}
			}
			else
			{
				if(playerProps->getX() < pp->getX())
				{
					playerProps->setVelocity(-10.0f,playerProps->getVelocityY());
					pp->setVelocity(10.0f,pp->getVelocityY());
				}
				else
				{
					playerProps->setVelocity(10.0f,playerProps->getVelocityY());
					pp->setVelocity(-10.0f,pp->getVelocityY());
				}

				health --;
			}
		}
		else if(b->isItem())
		{	
				codeCollected ++;
				game->getGSM()->getSpriteManager()->removeBot(b);
			
		}
		else if(b->isPortal())
		{
			won = true;
		}
	}
}

void SoSGameRules::spawnEnemies(Game* game)
{

}

void SoSGameRules::setUpGame(Game* game)
{
	health = 100;
	codeCollected = 0;
	doorX = 500;
	doorY = 1792;
	won = false;
	doorPlaced = false;
}