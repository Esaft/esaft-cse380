#include "stdafx.h"
#include "Physics.h"
#include "SSSF_SourceCode\game\Game.h"
#include "SSSF_SourceCode\gsm\sprite\SpriteManager.h"
#include "SSSF_SourceCode\gsm\state\GameStateManager.h"
#include "SSSF_SourceCode\gsm\world\TiledLayer.h"
#include "SSSF_SourceCode\gsm\physics\Collision.h"
#include "SSSF_SourceCode\gsm\physics\CollidableObject.h"
#include "SSSF_SourceCode\gsm\physics\BoundingVolume.h"
#include "SSSF_SourceCode\gsm\sprite\AnimatedSprite.h"

/*
	Default constructor, it initializes all data using default values.
*/
Physics::Physics()
{
	maxVelocity = DEFAULT_MAX_VELOCITY;
	gravity = DEFAULT_GRAVITY;

	/*for(int i = 0; i < 1000; i++)
	{
		Collision *c = new Collision();
		collisionStack.push(c);
	}//end for*/

	for(int i = 0; i < 1000; i++)
	{
		Collision *c = new Collision();
		collisionStack[i];
	}//end for
	
	collisionStackCounter = 999;
}

/*
	Noting to destroy at the moment.
*/
Physics::~Physics()
{

}

void Physics::update(Game *game)
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
	TiledLayer *tL;
	vector<Collision*> collisions;
	
	//finding TileLayer
	for(int i = 0; i < layers->size(); i++)
	{
		WorldLayer *currentLayer = (*layers)[i];
		if(currentLayer->hasCollidableTiles() == true)
		{
			tL = dynamic_cast<TiledLayer*>(currentLayer);
			if(tL != 0)
			{
				i = layers->size();
			}//end if
		}//end if
	}


	player = sm->getPlayer();
	pp = player->getPhysicalProperties();

	//UPDATING ALL VELOCITIES
	pp->incVelocity(this,pp->getAccelerationX(), pp->getAccelerationY() + gravity); 

	list<Bot*>::iterator botIterator = sm->getBotsIterator();
	while (botIterator != sm->getEndOfBotsIterator())
	{			
		Bot *bot = (*botIterator);
		pp = bot->getPhysicalProperties();
		pp->incVelocity(this, pp->getAccelerationX(), pp->getAccelerationY() + gravity);
		botIterator++;
	}


	/*pp->setPosition(pp->getX() + pp->getVelocityX(), pp->getY() + pp->getVelocityY());

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
	}*/

	
}

void Physics::collideTestWithTiles(AnimatedSprite *c,TiledLayer *tL, vector<Collision*> *collisions)
{
	
	

	BoundingVolume *bv = c->getBoundingVolume();
	float toLeft	= bv->getX() - bv->getWidth()/2;
	float toRight	= bv->getX() + bv->getWidth()/2;
	float toTop	 = bv->getY() - bv->getHeight()/2;
	float toBottom = bv->getY() + bv->getHeight()/2;

	PhysicalProperties *pp = c->getPhysicalProperties();
	float x = pp->getX();
	float y = pp->getY();
	float xVel = pp->getVelocityX();
	float yVel = pp->getVelocityY();
	float minX = x + toLeft;
	float maxX = x + toRight;
	float minY = y + toTop;
	float maxY = y + toBottom;

	if(xVel > 0)
		maxX += xVel;
	else
		minX += xVel;
	if(yVel > 0)
		maxY += yVel;
	else
		minY += yVel;

	unsigned int tW = tL->getTileWidth();
	unsigned int tH = tL->getTileHeight();

	int firstCol = minX/tW;
	int lastCol = maxX/tW;
	int firstRow = minY/tH;
	int lastRow = maxY/tH;

	for(int i = firstRow; i <= lastRow; i++)
	{
		for(int j = firstCol; j <= lastCol; j++)
		{
			Tile* current = tL->getTile(i,j);
			if(current->collidable == true)
			{
				if((i*tH > maxY || (i+1)*tH < minY)
					&& (j*tW > maxX && (j+1)*tW < minX))
				{
					CollidableObject* tileCO = new CollidableObject();
					BoundingVolume *bv = tileCO->getBoundingVolume();
					bv->setWidth(tW);
					bv->setHeight(tH);
					bv->setX(tW/2);
					bv->setY(tW/2);
					pp = tileCO->getPhysicalProperties();
					pp->setPosition(j*tW,i*tH);
					pp->setVelocity(0,0);

					/*
					Collision* currentCollision = collisionStack.top();
					collisionStack.pop();*/
					Collision* currentCollision = collisionStack[collisionStackCounter];
					collisionStackCounter --;


					currentCollision->setCO1(c);
					currentCollision->setCO2(tileCO);
					currentCollision->calculateTimes();

					collisions->push_back(currentCollision);
				}
			}
		}
	}
	
}