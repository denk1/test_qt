#ifndef WeaponLoader_H
#define WeaponLoader_H

#include <Ogre.h>
#include "tinyxml.h"
#include "Weapon.h"

namespace RAT
{
// Класс загружает оружие
class WeaponLoader
{
public:
	WeaponLoader();

	void parseWeapons(TiXmlElement* xmlNode, std::vector<WeaponDescription>& outParsed);

protected:

	void virtual parseWeapon(TiXmlElement* xmlNode, std::vector<WeaponDescription>& outParsed);

};

}

#endif