#include "WRocketLauncher.h"
#include "ITS.h"

namespace RAT
{

WRocketLauncher::WRocketLauncher(const WeaponDescription& desc)
	: Weapon(desc)
{

}

WRocketLauncher::~WRocketLauncher()
{
    ITS::destroyAllAttachedMovableObjects(ITS::getSceneManagerS(), mSceneNode);
	mSceneNode->removeAndDestroyAllChildren();
    ITS::getSceneManagerS()->destroySceneNode(mSceneNode);
}

void WRocketLauncher::create(Ogre::SceneNode* parentNode)
{
	mSceneNode = parentNode->createChildSceneNode(mDescription.mPosition);
    mSceneNode->attachObject(ITS::getSceneManagerS()->createEntity(mDescription.mMeshFile));
}


void WRocketLauncher::startShoot(const Ogre::Real dTime)
{
}

void WRocketLauncher::stopShoot(const Ogre::Real dTime)
{
}

void WRocketLauncher::update(const Ogre::Real dTime)
{
}

}
