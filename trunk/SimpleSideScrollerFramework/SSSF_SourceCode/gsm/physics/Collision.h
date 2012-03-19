
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

		CollidableObject* getCO1	{return co1;}
		CollidableObject* getCO2	{return co2;}
		float getTOF				{return timeOfCollision;}

		void setCO1(CollidableObject* co) {co1 = co;}
		void setCO2(CollidableObject* co) {co1 = co;}
		
};
