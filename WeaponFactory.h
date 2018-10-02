#ifndef WeaponFactory_H
#define WeaponFactory_H

#include <Ogre.h>
#include <OgreSingleton.h>
#include "Weapon.h"

namespace RAT
{

	// <Фабрика> оружия
class WeaponFactory : public Ogre::Singleton<WeaponFactory>
{
public:

	WeaponFactory();
	~WeaponFactory();

	// Создает оружие по описанию
	Weapon* buildWeapon(const WeaponDescription& desc);

protected:
	
	Weapon* buildGun(const WeaponDescription& desc);
	Weapon* buildMachineGun(const WeaponDescription& desc);
	Weapon* buildRocketLauncher(const WeaponDescription& desc);

	typedef Weapon* (WeaponFactory::*CreateWeaponPtr) (const WeaponDescription& desc);
	typedef std::map<Ogre::String, CreateWeaponPtr> TCreateMap;
	TCreateMap mCreateMap;

};

}

#endif