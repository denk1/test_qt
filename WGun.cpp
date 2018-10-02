#include "WGun.h"
#include "MyTestApp.h"

namespace RAT
{

WGun::WGun(const WeaponDescription& desc)
	: Weapon(desc)
{

}

WGun::~WGun()
{
    MyTestApp::destroyAllAttachedMovableObjects(MyTestApp::getSceneManagerS(), mSceneNode);
	mSceneNode->removeAndDestroyAllChildren();
    MyTestApp::getSceneManagerS()->destroySceneNode(mSceneNode);
}

void WGun::create(Ogre::SceneNode* parentNode)
{
	mSceneNode = parentNode->createChildSceneNode(mDescription.mPosition);
    mSceneNode->attachObject(MyTestApp::getSceneManagerS()->createEntity(mDescription.mMeshFile));
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
