
#pragma once
#include "stdafx.h"
#include "SSSF_SourceCode\gsm\physics\CollidableObject.h"

class Collision
{
	public:
		CollidableObject *co1;
		CollidableObject *co2;
		float timeOfCollision;
		float startTimeOfXCollision;
		float startTimeOfYCollision;
		float endTimeOfXCollision;
		float endTimeOfYCollision;
};
