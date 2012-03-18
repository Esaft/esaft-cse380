#pragma once
#include "stdafx.h"
#include "SSSF_SourceCode\gsm\physics\Physics.h"

class SoSPhysics : Physics
{
	public:
	SoSPhysics()	{}
	~SoSPhysics()	{}

	// METHODS TO BE DEFINED IN Physics.cpp
		
	void update(Game *game);
};