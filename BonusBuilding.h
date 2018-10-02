#ifndef BonusBuilding_H
#define BonusBuilding_H

#include <Ogre.h>
#include "BonusBuildingBase.h"
#include "GameObject.h"
#include "Physics.h"

namespace RAT
{

class BonusBuilding : public BonusBuildingBase, public GameObject
{
public:
	BonusBuilding(const BonusBuildingBase& base);
	~BonusBuilding();

	void create(Ogre::SceneNode* node);

		// Устанавливает начальное положение (Для изменения текущего, пользоваться SceneNode)
	void setPosition(const Ogre::Vector3& value);
	void setDirection(const Ogre::Vector3& value);

	virtual void update(const Ogre::Real dTime);

protected:

	Ogre::Vector3 mPosition;
	Ogre::Vector3 mDirection;
	
		// Используется только после вызова метода create()
	Ogre::SceneNode* mSceneNode;

	OgreBulletDynamics::RigidBody* mBody;
};

}

#endif