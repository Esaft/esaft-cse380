/*Abstract class that should be implemented by each specific game to specify specific game functions, like collision response,
win/lose conditions,  

*/

#include "stdafx.h"
#include "SSSF_SourceCode\gsm\physics\Collision.h"
#include "SSSF_SourceCode\game\Game.h"

class GameRules
{


	public:
		void GameSpecificResolve(Collision* c);
		void spawnEnemies(Game* game);
};