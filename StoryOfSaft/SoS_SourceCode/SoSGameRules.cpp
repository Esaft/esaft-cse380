#pragma once
#include "stdafx.h"
#include "SoSGameRules.h"
#include "SSSF_SourceCode\gsm\ai\Bot.h"
#include "SSSF_SourceCode\gsm\sprite\AnimatedSpriteType.h"
#include "SSSF_SourceCode\gsm\ai\SimpleTrackingBot.h"
#include "SSSF_SourceCode\gsm\physics\PhysicalProperties.h"
#include "SSSF_SourceCode\gsm\physics\BoundingVolume.h"



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

void SoSGameRules::gameSpecificResolve(Collision* c)
{
		
}

void SoSGameRules::spawnEnemies(Game* game)

{

}
