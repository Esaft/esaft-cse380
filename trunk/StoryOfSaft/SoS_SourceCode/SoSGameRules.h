#pragma once
#include "stdafx.h"
#include "SSSF_SourceCode\game\GameRules.h"
#include "SSSF_SourceCode\game\Game.h"
#include "SSSF_SourceCode\gsm\ai\Bot.h"
#include "SSSF_SourceCode\gsm\physics\CollidableObject.h"
#include "SSSF_SourceCode\gsm\sprite\AnimatedSprite.h"

class SoSGameRules: public GameRules
{
	private:
		vector<Bot*> *botTypeList; 
		int health;
		int codeCollected;
		int doorX;
		int doorY;
		bool won;

		void playerColResolve(AnimatedSprite* player, CollidableObject* other);
		void placeDoor(Game* game);

	public:
		SoSGameRules();
		~SoSGameRules();

		Bot* getBot(int type);

		void gameSpecificResolve(Game* game, Collision* c);
		void spawnEnemies(Game* game);
		void setBotSprite(AnimatedSpriteType* ast, int enemyType);
		void setUpGame(Game* game);
		void checkGameOver(Game* game){};
};