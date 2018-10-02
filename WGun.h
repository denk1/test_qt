#ifndef WGun_H
#define WGun_H

#include <Ogre.h>
#include "Weapon.h"

namespace RAT
{

class WGun: public Weapon
{
public:

	WGun(const WeaponDescription& desc);
	virtual ~WGun();

	// Создает оружие и помещает на сцену.
	virtual void create(Ogre::SceneNode* parentNode);

	// Начать стрелять
	virtual void startShoot(const Ogre::Real dTime);
	// Закончить стрелять
	virtual void stopShoot(const Ogre::Real dTime);
	// Обновление каждый кадр
	virtual void update(const Ogre::Real dTime);

};

}

#endif