#include "BonusBuildingBase.h"

namespace RAT
{
		
void Bonus::setType(const Ogre::String& value)
{
	mType = value;
}

void Bonus::setValue(int value)
{
	mValue = value;
}

//---


BonusBuildingBase::BonusBuildingBase()
{
}

BonusBuildingBase::~BonusBuildingBase()
{
	if (mShape)
		Physics::getSingleton().destroyShape(mShape);
}


void BonusBuildingBase::setBaseName(const Ogre::String& value)
{
	mBaseName = value;
}

void BonusBuildingBase::setBodyMesh(const Ogre::String& value)
{
	mBodyMesh = value;
}

void BonusBuildingBase::setResourceGroup(const Ogre::String& value)
{
	mResourceGroup = value;
}

void BonusBuildingBase::setTimeToCapture(int value)
{
	mTimeToCapture = value;
}

	
void BonusBuildingBase::addBonus(const Bonus& value)
{
	mBonuses.push_back(value);
}


Ogre::String BonusBuildingBase::getName()
{
	return mBaseName;
}

Ogre::String BonusBuildingBase::getBodyMesh()
{
	return mBodyMesh;
}


void BonusBuildingBase::setCollisionShape(OgreBulletCollisions::CollisionShape* shape )
{
	mShape = shape;
}

}
