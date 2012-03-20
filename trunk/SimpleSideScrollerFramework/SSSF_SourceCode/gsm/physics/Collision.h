
#pragma once
#include "stdafx.h"
#include "SSSF_SourceCode\gsm\physics\CollidableObject.h"
#include "SSSF_SourceCode\gsm\physics\PhysicalProperties.h"
#include "SSSF_SourceCode\gsm\physics\BoundingVolume.h"

class Collision
{
	private:
		CollidableObject *co1;
		CollidableObject *co2;
		float timeOfCollision;
		float startTimeOfXCollision;
		float startTimeOfYCollision;
		float endTimeOfXCollision;
		float endTimeOfYCollision;

	public:
		

		CollidableObject* getCO1()	{return co1;}
		CollidableObject* getCO2()	{return co2;}
		float getTOC()				{return timeOfCollision;}
		float getSXC()				{return startTimeOfXCollision;}
		float getEXC()				{return	endTimeOfXCollision;}
		float getSYC()				{return startTimeOfXCollision;}
		float getEYC()				{return endTimeOfXCollision;}
	

		void setCO1(CollidableObject* co) {co1 = co;}
		void setCO2(CollidableObject* co) {co1 = co;}
		
		bool contains(CollidableObject* c)
		{
			if(co1 == c || co2 == c)
				return true;
			return false;
		}

		void calculateTimes()
		{
			PhysicalProperties *pp;
			BoundingVolume *bv;

			pp = co1->getPhysicalProperties();
			bv = co1->getBoundingVolume();
			float xA = pp->getX() + bv->getX();
			float yA = pp->getY() + bv->getY();
			float widthA = bv->getWidth();
			float heightA = bv->getHeight();
			float velX = pp->getVelocityX();
			float velY = pp->getVelocityY();

			pp = co2->getPhysicalProperties();
			bv = co2->getBoundingVolume();
			float xB = pp->getX() + bv->getX();
			float yB = pp->getY() + bv->getY();
			float widthB = bv->getWidth();
			float heightB = bv->getHeight();
			velX -= pp->getVelocityX();
			velY -= pp->getVelocityY();

			if ((xB-(widthB/2)) > (xA+(widthB/2)))
			{
				startTimeOfXCollision = ((xB-(widthB/2)) - (xA + (widthA/2)))/velX;
				endTimeOfXCollision = ((xB + (widthB/2)) - (xA-(widthA/2)))/velX;
			}
			else if ((xB-(widthB/2)) < (xA+(widthB/2)))
			{
				startTimeOfXCollision = ((xB + (widthB/2)) - (xA-(widthA/2)))/velX;
				endTimeOfXCollision = ((xB-(widthB/2)) - (xA + (widthA/2)))/velX;
			}

			if ((yB-(heightB/2)) > (yA+(heightB/2)))
			{
				startTimeOfYCollision = ((yB-(heightB/2)) - (yA + (heightA/2)))/velY;
				endTimeOfYCollision = ((yB + (heightB/2)) - (yA-(heightA/2)))/velY;
			}
			else if ((yB-(heightB/2)) < (yA+(heightB/2)))
			{
				startTimeOfYCollision = ((yB + (heightB/2)) - (yA-(heightA/2)))/velY;
				endTimeOfYCollision = ((yB-(heightB/2)) - (yA + (heightA/2)))/velY;
			}
	

			if(startTimeOfXCollision >= startTimeOfYCollision && endTimeOfXCollision >= endTimeOfYCollision)
				timeOfCollision = startTimeOfXCollision;
			else if(startTimeOfXCollision < startTimeOfYCollision && endTimeOfXCollision < endTimeOfYCollision)
				timeOfCollision = startTimeOfYCollision;
			else
				timeOfCollision = 10;

		}
};
