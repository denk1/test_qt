#ifndef Vehicle_H
#define Vehicle_H

#include <Ogre.h>

#include "Physics.h"

#include "VehicleBase.h"
#include "WeaponHolder.h"

namespace RAT
{

class Vehicle : public VehicleBase
{
public:
	Vehicle(const VehiclePrototype& prototype);
	~Vehicle();

	virtual void create(Ogre::SceneNode* parentSN);

	virtual void connectWeapon(const int connectionPoint, WeaponHolder* weaponHolder);

	virtual bool frameRenderingQueued(const Ogre::FrameEvent& evt);

protected:
	typedef std::vector<WeaponHolder*> WeaponsVec;
	WeaponsVec	mWeapons;

	WeaponHolder* mControlledWeapon;

};

}

#endif