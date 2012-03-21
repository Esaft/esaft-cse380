
#pragma once
#include "stdafx.h"
#include "SSSF_SourceCode\gsm\physics\CollidableObject.h"
#include "SSSF_SourceCode\gsm\physics\BoundingVolume.h"
//class CollidableObject;
//class BoundingVolume;
class Collision
{
	private:
		CollidableObject* co1;
		CollidableObject* co2;
		float timeOfCollision;
		float startTimeOfXCollision;
		float startTimeOfYCollision;
		float endTimeOfXCollision;
		float endTimeOfYCollision;

	public:
		
		Collision()	{}
		~Collision() {}

		void setCO1(CollidableObject* co) {co1 = co;}
		void setCO2(CollidableObject* co) {co2 = co;}
	
		float getTOC()				{return timeOfCollision;}
		float getSXC()				{return startTimeOfXCollision;}
		float getEXC()				{return	endTimeOfXCollision;}
		float getSYC()				{return startTimeOfYCollision;}
		float getEYC()				{return endTimeOfYCollision;}
		CollidableObject* getCO1()	{return co1;}
		CollidableObject* getCO2()	{return co2;}

		
		
		bool contains(CollidableObject* c)
		{
			if(co1 == c || co2 == c)
				return true;
			return false;
		}

		void calculateTimes();
};
