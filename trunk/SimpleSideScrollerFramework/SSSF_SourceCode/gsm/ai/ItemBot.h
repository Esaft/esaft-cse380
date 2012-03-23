#include "stdafx.h"
#include "SSSF_SourceCode\game\Game.h"
#include "SSSF_SourceCode\gsm\sprite\AnimatedSprite.h"
#include "SSSF_SourceCode\gsm\ai\Bot.h"

class ItemBot : public Bot
{
private:

	

public:
	ItemBot() {item = true; enemy = false; portal = false;}
	~ItemBot() {}
	Bot*	clone() { ItemBot* botClone = new ItemBot();
								return botClone;}
	//void initBot();
	void	think(Game *game) {}

	// INLINED METHODS

};