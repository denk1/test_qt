#include "Weapon.h"

namespace RAT
{

Weapon::Weapon(const WeaponDescription& desc)
	: mDescription(desc)
{
}

Ogre::SceneNode* Weapon::getSceneNode()
{
	return mSceneNode;
}

const WeaponDescription&  Weapon::getDescription()
{
	return mDescription;
}

void Weapon::setOrientation(const Ogre::Radian angle)
{
	mSceneNode->setOrientation(Ogre::Quaternion(angle, Ogre::Vector3::UNIT_X));
}


}
