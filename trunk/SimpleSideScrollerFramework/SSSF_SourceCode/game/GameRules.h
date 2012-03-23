/*Abstract class that should be implemented by each specific game to specify specific game functions, like collision response,
win/lose conditions,  

*/

#include "stdafx.h"
#include "SSSF_SourceCode\gsm\physics\Collision.h"
#include "SSSF_SourceCode\game\Game.h"
#include "SSSF_SourceCode\gsm\sprite\AnimatedSpriteType.h"
#include "SSSF_SourceCode\gsm\ai\Bot.h"

class GameRules
{


	public:
		virtual void gameSpecificResolve(Game* game, Collision* c) = 0;
		virtual void setUpGame() = 0;
		virtual void spawnEnemies(Game* game) = 0;
		virtual void setBotSprite(AnimatedSpriteType* ast, int enemyType) = 0;
		virtual Bot* getBot(int type) = 0;
};