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
#include "SSSF_SourceCode\gsm\world\World.h"
#include "SSSF_SourceCode\gsm\world\TiledLayer.h"


SoSGameRules::SoSGameRules()
{
	srand (time(0));
	botTypeList = new vector<Bot*>();
	
	Bot* botToAdd;
	PhysicalProperties* pp;
	for(int i = 0; i < 4; i++)
	{
		if(i == 0)
		{
			botToAdd = new SimpleTrackingBot(2.0f,true,false);
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

	if(!c->getCO1()->isCurrentlyCollidable())
		return;
	if(!c->getCO2()->isCurrentlyCollidable())
		return;

	if(c->getCO1() == player)
	{
		playerColResolve(game, player, c->getCO2());
	}
	else if(c->getCO2() == player)
	{
		playerColResolve(game, player, c->getCO1());
	}

	if(health <= 0)
	{
		game->quitGame();
		game->getGSM()->goToGameOver();
	}
	if(codeCollected >= 5 && !doorPlaced)
	{
		placeDoor(game);
		doorPlaced = true;
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
			BoundingVolume* playerV = player->getBoundingVolume();
			BoundingVolume* bV = b->getBoundingVolume();
			

			if(player->getCurrentState().compare(L"ATTACK_STATE") == 0
           				|| player->getCurrentState().compare(L"ATTACKL_STATE") == 0 )
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
				playerProps->setStunned(true);
				if(playerProps->getX() < pp->getX())
				{
					playerProps->setVelocity(-5.0f,-2.0f);
					pp->setVelocity(10.0f,pp->getVelocityY());
				}
				else if(playerProps->getX() > pp->getX())
				{
					playerProps->setVelocity(5.0f,-2.0f);
					pp->setVelocity(-10.0f,pp->getVelocityY());
				}

				health --;
			}
		}
		else if(b->isItem())
		{	
			b->setCurrentlyCollidable(false);
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
	int generate = rand() % 100;
	Bot* toAdd = NULL;
	if(generate < 1)
	{
		toAdd = botTypeList->at(0)->clone();
	}
	if(generate >= 1 && generate <2)
	{
		toAdd = botTypeList->at(1)->clone();
	}
	if(toAdd != NULL)
	{
		TiledLayer* tL = game->getGSM()->getWorld()->getTiledLayer();
		Viewport* v = game->getGUI()->getViewport();

		int left = v->getViewportX();
		int width = v->getViewportWidth();
		int up = v->getViewportY();
		int height= v->getViewportHeight();
		int x;
		int y;
		Tile* t;
		do
		{
			x = ((rand() % width) + left)/64;
			y = ((rand() % height) + up)/64;
			t = tL->getTile(x,y);

		}while(t->collidable);
		
		PhysicalProperties* pp = toAdd->getPhysicalProperties();

		toAdd->setAlpha(255);
		toAdd->setCurrentState(L"RIGHT_STATE");
		toAdd->setCurrentlyCollidable(true);
		toAdd->setIsStatic(false);
		pp->setCollidable(true);
		pp->setGravAffected(true);
		pp->setX(x*64);
		pp->setY(y*64);

		game->getGSM()->getSpriteManager()->addBot(toAdd);
	}
}

void SoSGameRules::setUpGame(Game* game)
{
	health = 5;
	codeCollected = 0;
	doorX = 3520;
	doorY = 1280;
	won = false;
	doorPlaced = false;
}