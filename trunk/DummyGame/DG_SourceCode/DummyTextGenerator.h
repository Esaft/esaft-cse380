/*	
	Author: Richard McKenna
			Stony Brook University
			Computer Science Department

	DummyTextGenerator.h

	This class is a custom text generator for the EmptyGame
	application. Basically, by defining the initText and updateText
	methods, we can have it display whatever text we like.
*/

#pragma once
#include "stdafx.h"
#include "SSSF_SourceCode\text\TextGenerator.h"

class DummyTextGenerator : public TextGenerator
{
private:
	// THIS IS THE TEXT THIS OBJECT WILL UPDATE EACH FRAME
	wstring textToGenerate;

public:
	// NOTHING TO INITIALIZE OR DESTROY
	DummyTextGenerator()	{}
	~DummyTextGenerator()	{}

	// DEFINED IN DummyTextGenerator.cpp
	void appendTargetFPS(Game *game);
	void initText(Game *game);
	void updateText(Game *game);
};