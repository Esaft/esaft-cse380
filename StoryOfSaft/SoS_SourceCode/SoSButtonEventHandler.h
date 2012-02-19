/*	
	Author: Richard McKenna
			Stony Brook University
			Computer Science Department

	DummyButtonEventHandler.h

	This is a custom button event handler. Each game can choose
	to use different buttons and respond differently to when
	buttons are pressed. This class will handle button presses for
	the DummyGame following the ButtonEventHandler pattern.
*/

#pragma once
#include "stdafx.h"
#include "SSSF_SourceCode\input\ButtonEventHandler.h"

class Game;

class SoSButtonEventHandler: public ButtonEventHandler
{
public:
	// AS-IS, THIS CLASS HAS NO DATA, SO THERE IS NOTHING
	// TO INITIALIZE OR DESTROY
	SoSButtonEventHandler()	{}
	~SoSButtonEventHandler()	{}

	// DEFINED INSIDE DummyButtonEventHandler.cpp
	void handleButtonEvents(Game *game, 
							wstring command);
};