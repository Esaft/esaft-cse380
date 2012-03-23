/*	
	Author: _______________

	Physics.h

	This class will be used to manage all game collisions
	and physics. This includes collision detection, and resolution.
	Each frame we will update the collided objects velocities and
	positions accordingly.

	Students will implement this as part of their projects.
*/

#pragma once
#include "stdafx.h"
#include "SSSF_SourceCode\game\Game.h"
#include "SSSF_SourceCode\gsm\physics\Collision.h"
#include "SSSF_SourceCode\gsm\physics\CollidableObject.h"
#include "SSSF_SourceCode\gsm\world\TiledLayer.h"
#include "SSSF_SourceCode\gsm\sprite\AnimatedSprite.h"

const float DEFAULT_MAX_VELOCITY = 100.0f;
const float DEFAULT_GRAVITY = 0.1f;

class Physics
{
private:
	// USED TO APPLY TO GAME OBJECTS EACH FRAME
	float gravity;

	// THINK OF THIS AS THE WORLD'S TERMINAL VELOCITY
	float maxVelocity;
	

	//stack<Collision*> collisionStack;

	unsigned int coStackCounter;
	unsigned int collisionStackCounter;
	Collision* collisionStack[1000];
	CollidableObject* coStack[500];
	
	
	void collideTestWithTiles(CollidableObject *c,TiledLayer *tL, list<Collision*> *collisions);
	void collideTestWithSprites(Game* game, CollidableObject *s, list<Collision*> *collisions);
	void resolveCollision(Game* game, Collision* c);

public:
	// INLINED METHODS
	float	getGravity()							{ return gravity;					}
	float	getMaxVelocity()						{ return maxVelocity;				}
	void	setGravity(float initGravity)			{ gravity = initGravity;			}
	void	setMaxVelocity(float initMaxVelocity)	{ maxVelocity = initMaxVelocity;	}

	// METHODS TO BE DEFINED IN Physics.cpp
	Physics();
	~Physics();
	void update(Game *game);
};