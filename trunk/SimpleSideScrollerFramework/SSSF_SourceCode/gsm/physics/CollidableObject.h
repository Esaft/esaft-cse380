#pragma once
#include "stdafx.h"
#include "SSSF_SourceCode\gsm\physics\BoundingVolume.h"
#include "SSSF_SourceCode\gsm\physics\PhysicalProperties.h"

class CollidableObject
{
protected:
	bool currentlyCollidable;
	bool staticObject;
	bool mobile;
	bool enemy;
	bool item;
	bool portal;
	BoundingVolume bv;
	PhysicalProperties pp;

public:
	CollidableObject()	{enemy = false; item = false; portal = false;}
	~CollidableObject()	{}

	// INLINED METHODS
	bool				isCurrentlyCollidable()	{ return currentlyCollidable;	}
	bool				isStaticObject()		{ return staticObject;			}
	bool				isMobile()				{ return mobile;				}
	BoundingVolume*		getBoundingVolume()		{ return &bv;					}
	PhysicalProperties* getPhysicalProperties() { return &pp;					}

	void setCurrentlyCollidable(bool initCurrentlyCollidable)
	{	currentlyCollidable = initCurrentlyCollidable; }

	void setIsStatic(bool s)					{ staticObject = s;				}
	void setMobile(bool m)						{ mobile = m;					}
	void setEnemy(bool e)	{enemy = e;}
	void setItem(bool i)	{item = i;}
	void setPortal(bool p)  {portal = p;}

	bool isEnemy()	{return enemy;}
	bool isItem()	{return item;}
	bool isPortal()	{return portal;}
};