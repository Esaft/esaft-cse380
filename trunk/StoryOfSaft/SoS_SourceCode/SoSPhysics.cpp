#include "stdafx.h"
#include "SoSPhysics.h"
#include "SSSF_SourceCode\game\Game.h"
#include "SSSF_SourceCode\gsm\sprite\SpriteManager.h"
#include "SSSF_SourceCode\gsm\state\GameStateManager.h"

SoSPhysics::SoSPhysics() : Physics()
{

}

void SoSPhysics::update(Game *game)
{
	// REMEMBER, AT THIS POINT, ALL PLAYER INPUT AND AI
	// HAVE ALREADY BEEN PROCESSED AND BOT AND PLAYER
	// STATES, VELOCITIES, AND ACCELERATIONS HAVE ALREADY
	// BEEN UPDATED. NOW WE HAVE TO PROCESS THE PHYSICS
	// OF ALL THESE OBJECTS INTERACTING WITH EACH OTHER
	// AND THE STATIC GAME WORLD. THIS MEANS WE NEED TO
	// DETECT AND RESOLVE COLLISIONS IN THE ORDER THAT
	// THEY WILL HAPPEN, AND WITH EACH COLLISION, EXECUTE
	// ANY GAMEPLAY RESPONSE CODE, UPDATE VELOCITIES, AND
	// IN THE END, UPDATE POSITIONS

	// FIRST, YOU SHOULD START BY ADDING ACCELERATION TO ALL 
	// VELOCITIES, WHICH INCLUDES GRAVITY, NOTE THE EXAMPLE
	// BELOW DOES NOT DO THAT


	// FOR NOW, WE'LL JUST ADD THE VELOCITIES TO THE
	// POSITIONS, WHICH MEANS WE'RE NOT APPLYING GRAVITY OR
	// ACCELERATION AND WE ARE NOT DOING ANY COLLISION 
	// DETECTION OR RESPONSE
	GameStateManager *gsm = game->getGSM();
	SpriteManager *sm = gsm->getSpriteManager();
	World *w = gsm->getWorld();
	vector<WorldLayer*> *layers = w->getLayers();
	AnimatedSprite *player;
	PhysicalProperties *pp;

	// FIRST WE'LL MOVE THE PLAYER
	player = sm->getPlayer();
	pp = player->getPhysicalProperties();
	pp->incVelocity(this,pp->getAccelerationX(), pp->getAccelerationY() + gravity); 

	/*
	list<Bot*>::iterator botIterator = sm->getBotsIterator();
	while (botIterator != sm->getEndOfBotsIterator())
	{			
		Bot *bot = (*botIterator);
		pp = bot->getPhysicalProperties();
		pp->incVelocity(this, pp->getAccelerationX(), pp->getAccelerationY() + gravity);
		botIterator++;
	}*/


	pp->setPosition(pp->getX() + pp->getVelocityX(), pp->getY() + pp->getVelocityY());

	// FOR NOW THE PLAYER IS DIRECTLY CONTROLLED BY THE KEYBOARD,
	// SO WE'LL NEED TO TURN OFF ANY VELOCITY APPLIED BY INPUT
	// SO THE NEXT FRAME IT DOESN'T GET ADDED
	pp->setVelocity(0.0f, pp->getVelocityY());

	// AND NOW MOVE ALL THE BOTS
	list<Bot*>::iterator botIterator = sm->getBotsIterator();
	while (botIterator != sm->getEndOfBotsIterator())
	{			
		Bot *bot = (*botIterator);
		pp = bot->getPhysicalProperties();
		pp->setPosition(pp->getX() + pp->getVelocityX(), pp->getY() + pp->getVelocityY());
		botIterator++;
	}
}