#include "stdafx.h"
#include "SSSF_SourceCode\gsm\physics\Physics.h"
#include "SSSF_SourceCode\game\Game.h"
#include "SSSF_SourceCode\game\GameRules.h"
#include "SSSF_SourceCode\gsm\sprite\SpriteManager.h"
#include "SSSF_SourceCode\gsm\state\GameStateManager.h"
#include "SSSF_SourceCode\gsm\world\TiledLayer.h"
#include "SSSF_SourceCode\gsm\physics\Collision.h"
#include "SSSF_SourceCode\gsm\physics\CollidableObject.h"
#include "SSSF_SourceCode\gsm\physics\BoundingVolume.h"
#include "SSSF_SourceCode\gsm\sprite\AnimatedSprite.h"

bool compare_collisionTime(Collision *first, Collision *second)
{
	if(first->getTOC() < second->getTOC())
		return true;
	else
		return false;
}

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
		collisionStack[i] = c;

		if(i < 500)
		{
			CollidableObject* co = new CollidableObject();
			co->setCurrentlyCollidable(true);
			co->setIsStatic(true);
			PhysicalProperties* pp = co->getPhysicalProperties();
			pp->setVelocity(0,0);

			coStack[i] = co;
		}
	}//end for
	
	collisionStackCounter = 999;
	coStackCounter = 499;
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
	float timer = 0;

	GameStateManager *gsm = game->getGSM();
	SpriteManager *sm = gsm->getSpriteManager();
	World *w = gsm->getWorld();
	GameRules* gR = game->getGameRules();
	vector<WorldLayer*> *layers = w->getLayers();

	AnimatedSprite *player;
	PhysicalProperties *pp;
	TiledLayer *tL;
	list<Collision*> collisions;
	
	//finding TileLayer
	for(unsigned int i = 0; i < layers->size(); i++)
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

	//UPDATING ALL VELOCITIES AND DOING TILE COLLISION
	pp->incVelocity(this,pp->getAccelerationX(), pp->getAccelerationY() + gravity); 
	collideTestWithTiles(player, tL, &collisions);

	list<Bot*>::iterator botIterator = sm->getBotsIterator();
	while (botIterator != sm->getEndOfBotsIterator())
	{			
		Bot *bot = (*botIterator);
		pp = bot->getPhysicalProperties();
		pp->incVelocity(this, pp->getAccelerationX(), pp->getAccelerationY());
		if(pp->isGravAffected() == true)
			pp->incVelocity(this, 0, gravity);
		collideTestWithTiles(bot, tL, &collisions);
		botIterator++;
	}

	//HERE, COLLIDE SPRITES WITH OTHER SPRITES
	collideTestWithSprites(game, player, &collisions);
	botIterator = sm->getBotsIterator();
	while (botIterator != sm->getEndOfBotsIterator())
	{			
		Bot *bot = (*botIterator);
		if(bot->isCurrentlyCollidable() == true);
			collideTestWithSprites(game, bot, &collisions);
		botIterator++;
	}

	//SORT COLLISIONS
	collisions.sort(compare_collisionTime);

	//RESOLVING ALL THE COLLISIONS
	while(collisions.empty() == false)
	{
		Collision* currentCollision = collisions.front();
		collisions.pop_front();
		float colTime = currentCollision->getTOC();
		CollidableObject* co1 = currentCollision->getCO1();
		CollidableObject* co2 = currentCollision->getCO2();

		if(colTime >= 0 && colTime <= 1)
		{
			
			pp = co1->getPhysicalProperties();
			//pp->setVelocity(pp->getVelocityX()*9.99f,pp->getVelocityY()*9.99f);
			pp = co2->getPhysicalProperties();
			//pp->setVelocity(pp->getVelocityX()*9.99f,pp->getVelocityY()*9.99f);

			pp = player->getPhysicalProperties();
			pp->setPosition(pp->getX() + (pp->getVelocityX()*(colTime-timer)),pp->getY() + (pp->getVelocityY()*(colTime-timer)));
			botIterator = sm->getBotsIterator();
			while (botIterator != sm->getEndOfBotsIterator())
			{			
				Bot *bot = (*botIterator);
				pp = bot->getPhysicalProperties();
				pp->setPosition(pp->getX() + (pp->getVelocityX()*(colTime-timer)), pp->getY() + (pp->getVelocityY()*(colTime-timer)));
				botIterator++;
			}

			gsm->updateViewport(game, colTime-timer);

			resolveCollision(game, currentCollision);
			gR->gameSpecificResolve(game, currentCollision);
			
			boolean deleteLast = false;
			list<Collision*>::iterator cIterator = collisions.begin();
			list<Collision*>::iterator lastIterator;
			while(cIterator != collisions.end())
			{
				if(deleteLast == true)
				{
					collisions.erase(lastIterator);
				}
				deleteLast = false;
				Collision* check = (*cIterator);
				if(check->contains(co1) || check->contains(co2))
				{
					CollidableObject* checkStatic = check->getCO2();
					if(checkStatic->isStaticObject())
					{
						coStackCounter ++;
						coStack[coStackCounter] = checkStatic;
					}

					collisionStackCounter ++;
					collisionStack[collisionStackCounter] = check;

					lastIterator = cIterator;
					deleteLast = true;
				}
				else
				{
					//check->calculateTimes();
				}

				cIterator++;
			}

			if(deleteLast == true)
			{
				collisions.erase(lastIterator);
			}

			collideTestWithTiles(co1, tL, &collisions);
			collideTestWithSprites(game, co1, &collisions);

			if(co2->isStaticObject() == false)
			{
				collideTestWithTiles(co2, tL, &collisions);
				collideTestWithSprites(game, co2, &collisions);
			}

			collisions.sort(compare_collisionTime);

			timer += colTime;
		}//end if


		if(co2->isStaticObject() == true)
		{
			coStackCounter ++;
			coStack[coStackCounter] = co2;
		}

		collisionStackCounter ++;
		collisionStack[collisionStackCounter] = currentCollision;
	
	}
	
	if(timer < 1)
	{
		gsm->updateViewport(game, 1-timer);
		pp = player->getPhysicalProperties();
		pp->setPosition(pp->getX() + (pp->getVelocityX()*(1-timer)),pp->getY() + (pp->getVelocityY()*(1-timer)));
		//pp->setVelocity(0.0f, pp->getVelocityY());
		botIterator = sm->getBotsIterator();
		while (botIterator != sm->getEndOfBotsIterator())
		{			
			Bot *bot = (*botIterator);
			pp = bot->getPhysicalProperties();
			pp->setPosition(pp->getX() + (pp->getVelocityX()*(1-timer)), pp->getY() + (pp->getVelocityY()*(1-timer)));
			botIterator++;
		}
		gsm->updateViewport(game, 1-timer);
	}
	
	pp = player->getPhysicalProperties();
	if(pp->getX() < 0)
	{
		pp->setX(0);
	}
	if(pp->getY() < 0)
	{
		pp->setY(0);
	}
	//pp->setVelocity(0.0f, pp->getVelocityY());
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

void Physics::collideTestWithTiles(CollidableObject *c,TiledLayer *tL, list<Collision*> *collisions)
{
	BoundingVolume *bv = c->getBoundingVolume();
	float toRight= bv->getWidth()/2;
	float toBottom = bv->getHeight()/2;

	PhysicalProperties *pp = c->getPhysicalProperties();
	float x = pp->getX()+bv->getX();
	float y = pp->getY()+bv->getY();
	float xVel = pp->getVelocityX();
	float yVel = pp->getVelocityY();
	float minX = x - toRight;
	float maxX = x + toRight;
	float minY = y - toBottom;
	float maxY = y + toBottom;

	if(xVel > 0)
		maxX += xVel;
	else
		minX += xVel;
	if(yVel > 0)
		maxY += yVel;
	else
		minY += yVel;

	 int tW = tL->getTileWidth();
	 int tH = tL->getTileHeight();

	int firstCol = minX/tW;
	int lastCol = maxX/tW;
	int firstRow = minY/tH;
	int lastRow = maxY/tH;

	if(firstCol < 0)
		firstCol = 0;
	if(firstRow < 0)
		firstRow = 0;
	if(lastCol >= tL->getColumns())
		lastCol = tL->getColumns() - 1;
	if(lastRow >= tL->getRows())
		lastRow = tL->getRows() - 1;

	for(int i = firstRow; i <= lastRow; i++)
	{
		for(int j = firstCol; j <= lastCol; j++)
		{
			Tile* current = tL->getTile(i,j);
			if(current->collidable == true)
			{
				if( !( (i+1)*tH < minY || i*tH > maxY || (j+1)*tW < minX || j*tW > maxX)  )
				{
					CollidableObject* tileCO = coStack[coStackCounter];
					coStackCounter --;
					BoundingVolume *bv = tileCO->getBoundingVolume();
					bv->setWidth(tW);
					bv->setHeight(tH);
					bv->setX(tW/2);
					bv->setY(tW/2);
					pp = tileCO->getPhysicalProperties();
					pp->setPosition(j*tW,i*tH);
					

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


//FOR NOW, TESTS SPRITES AGAINST ALL OTHER SPRITES. IF I HAVE TIME, I WILL IMPLEMENT SWEEP AND PRUNE
void Physics::collideTestWithSprites(Game* game, CollidableObject* c, list<Collision*> *collisions)
{
	SpriteManager *sm = game->getGSM()->getSpriteManager();
	AnimatedSprite* player = sm->getPlayer();

	if(c != player && player->isCurrentlyCollidable() == true)
	{
		if(willObjectsCollide(player, c) == true)
		{
			Collision* currentCollision = collisionStack[collisionStackCounter];
			collisionStackCounter --;
			currentCollision->setCO1(player);
			currentCollision->setCO2(c);
			currentCollision->calculateTimes();

			if(currentCollision->getTOC() > 0 && currentCollision->getTOC() <= 1)
			{
				collisions->push_back(currentCollision);
			}
			else
			{
				collisionStackCounter ++;
				collisionStack[collisionStackCounter] = currentCollision;
			}
		}
	}

	list<Bot*>::iterator botIterator = sm->getBotsIterator();
	while (botIterator != sm->getEndOfBotsIterator())
	{			
		Bot *bot = (*botIterator);
		if(c != bot && bot->isCurrentlyCollidable() == true)
		{
			if(willObjectsCollide(c, bot) == true)
			{
				Collision* currentCollision = collisionStack[collisionStackCounter];
				collisionStackCounter --;
				currentCollision->setCO1(c);
				currentCollision->setCO2(bot);
				currentCollision->calculateTimes();

				if(currentCollision->getTOC() > 0 && currentCollision->getTOC() <= 1)
				{
					collisions->push_back(currentCollision);
				}
				else
				{
					collisionStackCounter ++;
					collisionStack[collisionStackCounter] = currentCollision;
				}
			}
		}
		
		botIterator++;
	}
	/*Collision* currentCollision = collisionStack[collisionStackCounter];
	collisionStackCounter --;


	currentCollision->setCO1(c);
	currentCollision->setCO2(tileCO);
	currentCollision->calculateTimes();

	collisions->push_back(currentCollision);*/

}

bool Physics::willObjectsCollide(CollidableObject* coA, CollidableObject* coB)
{
	PhysicalProperties* pp = coA->getPhysicalProperties();
	BoundingVolume* bv = coA->getBoundingVolume();
	float widthA = bv->getWidth();
	float heightA = bv->getHeight();
	float xA = pp->getX() + bv->getX();
	float yA = pp->getY() + bv->getY();
	float velXA = pp->getVelocityX();
	float velYA = pp->getVelocityY();
	float minXA = xA-(widthA/2);
	float maxXA = xA+(widthA/2);
	float minYA = yA-(heightA/2);
	float maxYA = yA+(heightA/2);

	if(velXA >= 0)
		maxXA += velXA; 
	if(velXA < 0)
		minXA += velXA; 
	if(velYA >= 0)
		maxYA += velYA; 
	if(velYA < 0)
		minYA += velYA; 

	pp = coB->getPhysicalProperties();
	bv = coB->getBoundingVolume();
	float widthB = bv->getWidth();
	float heightB = bv->getHeight();
	float xB = pp->getX() + bv->getX();
	float yB = pp->getY() + bv->getY();
	float velXB = pp->getVelocityX();
	float velYB = pp->getVelocityY();
	float minXB = xB-(widthB/2);
	float maxXB = xB+(widthB/2);
	float minYB = yB-(heightB/2);
	float maxYB = yB+(heightB/2);

	if(velXB >= 0)
		maxXB += velXB; 
	if(velXB < 0)
		minXB += velXB; 
	if(velYB >= 0)
		maxYB += velYB; 
	if(velYB < 0)
		minYB += velYB; 

	if( !(maxXB < minXA || minXB > maxXA || maxYB < minYA || minYB > maxYA))
		return true;
	return false;
}

void Physics::resolveCollision(Game* game, Collision* currentCollision)
{
	CollidableObject* co1 = currentCollision->getCO1();
	CollidableObject* co2 = currentCollision->getCO2();
	PhysicalProperties* pp;
	BoundingVolume* bv;
	AnimatedSprite* player = game->getGSM()->getSpriteManager()->getPlayer();
	
	if(co2->isStaticObject() == true)
	{
		pp = co2->getPhysicalProperties();
		bv = co2->getBoundingVolume();
		float tX = pp->getX();
		float tY = pp->getY();
		float tXR = tX + bv->getWidth();
		float tYB = tY + bv->getHeight();

		pp = co1->getPhysicalProperties();
		bv = co1->getBoundingVolume();
		float x = pp->getX() + bv->getX() - (bv->getWidth()/2);
		float y = pp->getY() + bv->getY() - (bv->getHeight()/2);
		float xR = x+bv->getWidth();
		float yB = y+bv->getHeight();
		//pp->setVelocity(0, 0);

		/*if(x < tX)
			pp->setX(pp->getX() - 0.1);
		if(x > tX)
			pp->setX(pp->getX() + 0.1);*/
		if(x >= tXR)
		{
			pp->setX(pp->getX() + 0.1);
			pp->setVelocity(0, pp->getVelocityY());
		}
		if(xR <= tX)
		{
			pp->setX(pp->getX() - 0.1);
			pp->setVelocity(0, pp->getVelocityY());
		}
		if(y >= tYB)
		{
			pp->setY(pp->getY() + 0.1);
			pp->setVelocity(pp->getVelocityX(), 0);
		}
		if(yB <= tY)
		{
			pp->setY(pp->getY() - 0.1);
				if(co1 == player)
				{
					pp->setJumped(false);
					pp->setDoubleJumped(false);
				}
				pp->setVelocity(pp->getVelocityX(), 0);
		}

		//if(currentCollision->getTOC() == currentCollision->getSYC())
		//{
		//	/*if(pp->getVelocityY() < 0)
		//	{
		//		pp->setY(pp->getY() + 0.1);
		//	}
		//	else
		//	{
		//		pp->setY(pp->getY() - 0.1);
		//		if(co1 == player)
		//		{
		//			pp->setJumped(false);s
		//			pp->setDoubleJumped(false);
		//			if(player->getCurrentState().compare(L"JUMP_STATE") == 0 
		//				|| player->getCurrentState().compare(L"JUMPL_STATE") == 0)
		//			{
		//				player->setCurrentState(L"IDLE_STATE");
		//			}
		//		}
		//	}*/
		//	if(y < tY)
		//	{
		//		pp->setY(pp->getY() - 0.1);
		//		if(co1 == player)
		//		{
		//			pp->setJumped(false);
		//			pp->setDoubleJumped(false);
		//			/*if(player->getCurrentState().compare(L"JUMP_STATE") == 0 
		//				|| player->getCurrentState().compare(L"JUMPL_STATE") == 0)
		//			{
		//				player->setCurrentState(L"IDLE_STATE");
		//			}*/
		//		}
		//	}
		//	if(y > tY)
		//	{
		//		pp->setY(pp->getY() + 0.1);
		//		
		//	}
		//	pp->setVelocity(pp->getVelocityX(), 0);
		//	
		//	
		//}
		//else if(currentCollision->getTOC() == currentCollision->getSXC())
		//{
		//	/*if(pp->getVelocityX() < 0)
		//	{
		//		pp->setX(pp->getX() + 0.1);
		//	}
		//	else
		//	{
		//		pp->setX(pp->getX() - 0.1);
		//	}*/
		//	/*if(x < tX)
		//	pp->setX(pp->getX() - 0.1);
		//	if(x > tX)
		//	pp->setX(pp->getX() + 0.1);*/
		//	pp->setVelocity(0, pp->getVelocityY());
		//}
		//else
		//{
		//	/*if(pp->getVelocityY() < 0)
		//		pp->setY(pp->getY() + 0.1);
		//	else
		//		pp->setY(pp->getY() - 0.1);

		//	if(pp->getVelocityX() < 0)
		//		pp->setX(pp->getX() + 0.1);
		//	else
		//		pp->setX(pp->getX() - 0.1);*/

		//	pp->setVelocity(0,0);
		//}
	}
}



