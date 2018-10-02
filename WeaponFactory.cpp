#include "WeaponFactory.h"
#include "WGun.h"
#include "WRocketLauncher.h"


template<> RAT::WeaponFactory* Ogre::Singleton<RAT::WeaponFactory>::msSingleton = 0;

namespace RAT
{

WeaponFactory::WeaponFactory()
{
	mCreateMap["gun"] = &WeaponFactory::buildGun;
	mCreateMap["machineGun"] = &WeaponFactory::buildMachineGun;
	mCreateMap["rocketLauncher"] = &WeaponFactory::buildRocketLauncher;
}

WeaponFactory::~WeaponFactory()
{
}

Weapon* WeaponFactory::buildWeapon(const WeaponDescription& desc)
{
	Weapon* retval = 0;
	TCreateMap::iterator it = mCreateMap.find(desc.mType);
	if (it != mCreateMap.end())
	{
		return (this->*it->second)( desc);
	}

	return 0;
}

Weapon* WeaponFactory::buildGun(const WeaponDescription& desc)
{
	Weapon* retval = new WGun(desc);
	return retval;
}

Weapon* WeaponFactory::buildMachineGun(const WeaponDescription& desc)
{
	return 0;
}

Weapon* WeaponFactory::buildRocketLauncher(const WeaponDescription& desc)
{
	Weapon* retval = new WRocketLauncher(desc);
	return retval;
}

}
