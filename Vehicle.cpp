#include "Vehicle.h"
#include "MyTestApp.h"
#include "WeaponFactory.h"

namespace RAT
{

Vehicle::Vehicle(const VehiclePrototype& prototype)
	: VehicleBase(prototype)
	, mControlledWeapon(0)
	, mWeapons(mConnectionPoints.size(), 0)
{
}

Vehicle::~Vehicle()
{
	for(WeaponsVec::iterator it = mWeapons.begin(); it != mWeapons.end(); ++it)
		if (*it)
			delete *it;
}

void Vehicle::create(Ogre::SceneNode* parentSN)
{
	VehicleBase::create(parentSN);
}

void Vehicle::connectWeapon(const int connectionPoint, WeaponHolder* weaponHolder)
{
	if (connectionPoint >= mWeapons.size())
		return;

		// Удаляем старое оружие
	if (mWeapons[connectionPoint])
		delete mWeapons[connectionPoint];

	mWeapons[connectionPoint] = weaponHolder;
	if (!mConnectionPoints[connectionPoint].mIsStatic)
		mControlledWeapon = weaponHolder;

	weaponHolder->create(mCPNodes[connectionPoint]);
}

bool Vehicle::frameRenderingQueued(const Ogre::FrameEvent& evt)
{
	for(WeaponsVec::iterator it = mWeapons.begin(); it != mWeapons.end(); ++it)
		if (*it)
			(*it)->update(evt.timeSinceLastFrame);

	
	if (mControlledWeapon)
	{
		const Ogre::Quaternion& camRot = TPCamera::getSingleton().getCamera()->getOrientation();
		Ogre::Degree yaw = camRot.getYaw() - mControlledWeapon->getSceneNode()->getParentSceneNode()->_getDerivedOrientation().getYaw();
			// Вычисляем угол наклона камеры
		Ogre::Vector3 direction = TPCamera::getSingleton().getCamera()->getDirection();
		Ogre::Real len = direction.length();
		Ogre::Degree pitch(Ogre::Radian(Ogre::Math::ASin(direction.y / len)));

		mControlledWeapon->setOrientation(yaw, pitch);

	}

	return VehicleBase::frameRenderingQueued(evt);
}

}
