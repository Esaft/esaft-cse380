/*	
	Author: Richard McKenna
			Stony Brook University
			Computer Science Department

	DummyKeyEventHandler.h

	This is a custom key event handler. Each game can choose
	to use different keyboard keys and respond differently to when
	they are pressed. This class will handle key presses for
	this particular game following the KeyEventHandler pattern.
*/

#pragma once
#include "stdafx.h"
#include "SSSF_SourceCode\input\KeyEventHandler.h"

class Game;

class DummyKeyEventHandler : public KeyEventHandler
{
public:
	DummyKeyEventHandler()		{}
	~DummyKeyEventHandler()		{}
	void handleKeyEvents(Game *game);
};