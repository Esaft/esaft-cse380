#pragma once
#include "stdafx.h"
#include "SSSF_SourceCode\game\GameRules.h"
#include "SSSF_SourceCode\gsm\ai\Bot.h"

class SoSGameRules: public GameRules
{
	private:
		vector<Bot*> *botTypeList; 

	public:
		SoSGameRules();
		~SoSGameRules();

		Bot* getBot(int type);

		void gameSpecificResolve(Collision* c);
		void spawnEnemies(Game* game);
		void setBotSprite(AnimatedSpriteType* ast, int enemyType);
		

};