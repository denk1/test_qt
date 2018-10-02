#ifndef TowerBase_H
#define TowerBase_H

#include <Ogre.h>
#include "Landscape.h"
#include "Physics.h"
#include "Weapon.h"
#include "WeaponHolder.h"

namespace RAT
{



class TowerBase
{
public:
	TowerBase();
	TowerBase(const TowerBase&);
	virtual ~TowerBase();

	void setBaseName(const Ogre::String& value);
	void setBaseLife(int value);
	void setRespawnTime(int value);
	void setUpgradeTime(int value);

	void setResourceGroup(const Ogre::String& value);
	void setBodyMesh(const Ogre::String& value);

	void setWeaponHolder(WeaponHolder* holder);
	void addWeaponDescription(const WeaponDescription& value);

	Ogre::String getName();
	Ogre::String getBodyMesh();

	void setCollisionShape(OgreBulletCollisions::CollisionShape* shape );
	
protected:
	int mBaseLife;
	int mRespawnTime;
	int mUpgradeTime;
	Ogre::String mBaseName;
	Ogre::String mResourceGroup;
	Ogre::String mBodyMesh;

	OgreBulletCollisions::CollisionShape* mShape;
	
	WeaponHolder* mWeaponHolder;
	
};

}

#endif