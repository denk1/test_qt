#ifndef WeaponHolder_H
#define WeaponHolder_H

#include <Ogre.h>
#include "Weapon.h"

namespace RAT
{

struct HolderDescription
{
	Ogre::String	mMeshFile;
	float			mMaxAngle;
	float			mRotateSpeed;
	Ogre::Vector3	mPosition;

	HolderDescription()
		: mMeshFile("")
		, mMaxAngle(0)
		, mRotateSpeed(0)
		, mPosition(0,0,0)
	{
	}
};

	// Крепление с оружием
class WeaponHolder
{
public:

	WeaponHolder();
	WeaponHolder(const WeaponHolder&);

	virtual ~WeaponHolder();

	void setDesctiption(const HolderDescription& desc);
	const HolderDescription& getDescription();

	void addWeapon(Weapon* weapon);

	void create(Ogre::SceneNode* parentNode);

	virtual Ogre::SceneNode* getSceneNode();

	virtual void setOrientation(const Ogre::Radian yaw, const Ogre::Radian pitch);

	// Начать стрелять
	virtual void startShoot(const Ogre::Real dTime);
	// Закончить стрелять
	virtual void stopShoot(const Ogre::Real dTime);
	// Обновление каждый кадр
	virtual void update(const Ogre::Real dTime);

protected:
	HolderDescription mDescription;
	std::vector<Weapon*> mWeapons;

	Ogre::SceneNode* mSceneNode;
};

}

#endif

