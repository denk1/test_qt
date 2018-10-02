#include "TowerBase.h"

namespace RAT
{

TowerBase::TowerBase()
	: mBaseLife(0)
	, mRespawnTime(0)
	, mUpgradeTime(0)
	, mResourceGroup("General")
	, mBodyMesh("")
	, mShape(0)
	, mWeaponHolder(0)
{

}

TowerBase::TowerBase(const TowerBase& towerBase)
	: mBaseLife(towerBase.mBaseLife)
	, mRespawnTime(towerBase.mRespawnTime)
	, mUpgradeTime(towerBase.mUpgradeTime)
	, mResourceGroup(towerBase.mResourceGroup)
	, mBodyMesh(towerBase.mBodyMesh)
	, mShape(towerBase.mShape)
	, mWeaponHolder(new WeaponHolder(*towerBase.mWeaponHolder))
{
}

TowerBase::~TowerBase()
{
	if (mShape)
	{
		Physics::getSingleton().destroyShape(mShape);
		mShape = 0;
	}
}

void TowerBase::setBaseLife(int value)
{
	mBaseLife = value;
}
void TowerBase::setRespawnTime(int value)
{
	mRespawnTime = value;
}

void TowerBase::setUpgradeTime(int value)
{
	mUpgradeTime = value;
}

void TowerBase::setBaseName(const Ogre::String& value)
{
	mBaseName = value;
}

void TowerBase::setResourceGroup(const Ogre::String& value)
{
	mResourceGroup = value;
}

void TowerBase::setBodyMesh(const Ogre::String& value)
{
	mBodyMesh = value;
}


void TowerBase::setWeaponHolder(WeaponHolder* holder)
{
	mWeaponHolder = holder;
}

Ogre::String TowerBase::getName()
{
	return mBaseName;
}

void TowerBase::setCollisionShape(OgreBulletCollisions::CollisionShape* shape )
{
	mShape = shape;
}

Ogre::String TowerBase::getBodyMesh()
{
	return mBodyMesh;
}

}
