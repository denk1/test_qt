#ifndef BonusBuildingBase_H
#define BonusBuildingBase_H

#include <Ogre.h>
#include "Landscape.h"
#include "Physics.h"
#include "TowerBase.h"

namespace RAT
{

class Bonus
{
public:
	
	void setType(const Ogre::String& value);
	void setValue(int value);

protected:
	Ogre::String mType;
	int mValue;
};

class BonusBuildingBase
{
public:
	BonusBuildingBase();
	virtual ~BonusBuildingBase();

	void setBaseName(const Ogre::String& value);
	void setTimeToCapture(int value);
	void setBodyMesh(const Ogre::String& value);
	void setResourceGroup(const Ogre::String& value);

	void addBonus(const Bonus& value);

	Ogre::String getName();
	Ogre::String getBodyMesh();

	void setCollisionShape(OgreBulletCollisions::CollisionShape* shape );
	
protected:
	int mTimeToCapture;

	Ogre::String mBaseName;
	Ogre::String mResourceGroup;
	Ogre::String mBodyMesh;

	OgreBulletCollisions::CollisionShape* mShape;

	std::vector<Bonus> mBonuses;

	//typedef std::map<Ogre::String, TowerBase*> TowerBaseMap;
	//TowerBaseMap mTowerBases;

};

}

#endif