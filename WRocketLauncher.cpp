#include "WRocketLauncher.h"
#include "MyTestApp.h"

namespace RAT
{

WRocketLauncher::WRocketLauncher(const WeaponDescription& desc)
	: Weapon(desc)
{

}

WRocketLauncher::~WRocketLauncher()
{
    MyTestApp::destroyAllAttachedMovableObjects(MyTestApp::getSceneManagerS(), mSceneNode);
	mSceneNode->removeAndDestroyAllChildren();
    MyTestApp::getSceneManagerS()->destroySceneNode(mSceneNode);
}

void WRocketLauncher::create(Ogre::SceneNode* parentNode)
{
	mSceneNode = parentNode->createChildSceneNode(mDescription.mPosition);
    mSceneNode->attachObject(MyTestApp::getSceneManagerS()->createEntity(mDescription.mMeshFile));
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
