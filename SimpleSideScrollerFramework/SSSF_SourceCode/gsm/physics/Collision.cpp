#include "stdafx.h"
#include "SSSF_SourceCode\gsm\physics\Collision.h"
#include "SSSF_SourceCode\gsm\physics\CollidableObject.h"
#include "SSSF_SourceCode\gsm\physics\PhysicalProperties.h"


		void Collision::calculateTimes()
		{
			PhysicalProperties* pp;
			BoundingVolume* bv;

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
	

			if(startTimeOfXCollision > startTimeOfYCollision)
			{
				if(startTimeOfXCollision <= 1 && startTimeOfXCollision > 0)
					timeOfCollision = startTimeOfXCollision;
				else if(startTimeOfYCollision > 0 && startTimeOfYCollision <= 1)
					timeOfCollision = startTimeOfYCollision;
			}
			else if(startTimeOfXCollision <= startTimeOfYCollision)
			{
				 if(startTimeOfYCollision > 0 && startTimeOfYCollision <= 1)
					timeOfCollision = startTimeOfYCollision;
				 else if(startTimeOfXCollision <= 1 && startTimeOfXCollision > 0)
					 timeOfCollision = startTimeOfXCollision;
			}
			else
				timeOfCollision = 10;
		
		}