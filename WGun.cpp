#include "WGun.h"
#include "ITS.h"

namespace RAT
{

WGun::WGun(const WeaponDescription& desc)
	: Weapon(desc)
{

}

WGun::~WGun()
{
    ITS::destroyAllAttachedMovableObjects(ITS::getSceneManagerS(), mSceneNode);
	mSceneNode->removeAndDestroyAllChildren();
    ITS::getSceneManagerS()->destroySceneNode(mSceneNode);
}

void WGun::create(Ogre::SceneNode* parentNode)
{
	mSceneNode = parentNode->createChildSceneNode(mDescription.mPosition);
    mSceneNode->attachObject(ITS::getSceneManagerS()->createEntity(mDescription.mMeshFile));
}


void  WGun::startShoot(const Ogre::Real dTime)
{
}

void  WGun::stopShoot(const Ogre::Real dTime)
{
}

void  WGun::update(const Ogre::Real dTime)
{
}

}
