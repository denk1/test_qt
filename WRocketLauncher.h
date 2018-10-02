#ifndef WRocketLauncher_H
#define WRocketLauncher_H

#include <Ogre.h>
#include "Weapon.h"

namespace RAT
{

class WRocketLauncher: public Weapon
{
public:

	WRocketLauncher(const WeaponDescription& desc);
	virtual ~WRocketLauncher();

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