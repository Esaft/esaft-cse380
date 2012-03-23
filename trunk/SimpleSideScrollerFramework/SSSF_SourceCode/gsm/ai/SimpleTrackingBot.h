#pragma once

// PREPROCESSOR INCLUDE STATEMENTS
#include "stdafx.h"
#include "SSSF_SourceCode\gsm\ai\Bot.h"
#include "SSSF_SourceCode\gsm\physics\Physics.h"
#include "SSSF_SourceCode\game\Game.h"


//const float paths[]  =  {-5,0,10, 5,0,20, -5,-5,10, 0,0,10, 0,20,5, -10,0,5, 5,-5,10, 0,0,10 };//Stores sets of 3 numbers: path X velocity, path Y velocity, path duration

class SimpleTrackingBot : public Bot
{
private:
	//unsigned int cyclesRemainingBeforeChange;
	//unsigned int pathCounter;
	//unsigned int pathSize;
	float velMag; //velocity magnitude
	bool trackX;
	bool trackY;

	// THIS PRIVATE CONSTRUCTOR IS ONLY USED FOR CLONING
	//SimpleTrackingBot();

public:
	SimpleTrackingBot(float vM, bool tX, bool tY);
	~SimpleTrackingBot() {}
	Bot*	clone();
	//void initBot();
	//void	pickNextVelocity(Physics *physics);
	void	setTrackX(bool tX)	{trackX = tX;}
	void	setTrackY(bool tY)	{trackY = tY;}
	void	setVelMag(float vM) {velMag = vM;}
	void	think(Game *game);
	void	setDead(bool d);
	// INLINED METHODS
	//unsigned int getCyclesRemainingBeforeChange()	{ return cyclesRemainingBeforeChange; }

};