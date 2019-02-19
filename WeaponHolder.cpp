#include "WeaponHolder.h"
#include "ITS.h"
#include "WeaponFactory.h"

namespace RAT
{

WeaponHolder::WeaponHolder()
    : mSceneNode(0)
{
}

WeaponHolder::WeaponHolder(const WeaponHolder& holder)
    : mSceneNode(0)
    , mDescription(holder.mDescription)
{
    // ??????? ????? ?????
    for(std::vector<Weapon*>::const_iterator it = holder.mWeapons.begin(); it != holder.mWeapons.end(); ++it)
    {
        Weapon* weapon = WeaponFactory::getSingleton().buildWeapon((*it)->getDescription());
        mWeapons.push_back(weapon);
    }
}


WeaponHolder::~WeaponHolder()
{
    // ??????? ?????
    for(std::vector<Weapon*>::iterator it= mWeapons.begin(); it != mWeapons.end(); ++it)
    {
        delete *it;
    }

    if (mSceneNode)
    {
        // ?????????? ??????????? ?????????
        ITS::destroyAllAttachedMovableObjects(ITS::getSceneManagerS(), mSceneNode);
        mSceneNode->removeAndDestroyAllChildren();
        ITS::getSceneManagerS()->destroySceneNode(mSceneNode);
    }
}

void WeaponHolder::setDesctiption(const HolderDescription& desc)
{
    mDescription = desc;
}

const HolderDescription& WeaponHolder::getDescription()
{
    return mDescription;
}

void WeaponHolder::create(Ogre::SceneNode* parentNode)
{
    mSceneNode = parentNode->createChildSceneNode(mDescription.mPosition);
    mSceneNode->attachObject(ITS::getSceneManagerS()->createEntity(mDescription.mMeshFile));

    for(std::vector<Weapon*>::iterator it= mWeapons.begin(); it != mWeapons.end(); ++it)
    {
        (*it)->create(mSceneNode);
    }
}

void  WeaponHolder::startShoot(const Ogre::Real dTime)
{
}

void  WeaponHolder::stopShoot(const Ogre::Real dTime)
{
}

Ogre::SceneNode* WeaponHolder::getSceneNode()
{
    return mSceneNode;
}

void WeaponHolder::setOrientation(const Ogre::Radian yaw, const Ogre::Radian pitch)
{
    mSceneNode->setOrientation(Ogre::Quaternion(yaw, Ogre::Vector3::UNIT_Y));

    for(std::vector<Weapon*>::iterator it= mWeapons.begin(); it != mWeapons.end(); ++it)
    {
        (*it)->setOrientation(pitch);
    }
}

void  WeaponHolder::update(const Ogre::Real dTime)
{
    for(std::vector<Weapon*>::iterator it= mWeapons.begin(); it != mWeapons.end(); ++it)
    {
        (*it)->update(dTime);
    }
}

void WeaponHolder::addWeapon(Weapon* weapon)
{
    mWeapons.push_back(weapon);
}

}
