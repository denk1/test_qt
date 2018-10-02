#include "Tower.h"
#include "MyTestApp.h"
#include "WeaponFactory.h"

namespace RAT
{

Tower::Tower(const TowerBase& base)
	:TowerBase(base), GameObject(GameObject::Tower), mTowerSceneNode(0)
{

}

Tower::~Tower()
{
	Physics::getSingleton().destroyRigidBody(mBody);

	/*for(std::vector<Weapon*>::iterator it= mWeapons.begin(); it != mWeapons.end(); ++it)
	{
		delete *it;
	}
	mWeapons.clear();*/
	if (mTowerSceneNode)
	{
        MyTestApp::destroyAllAttachedMovableObjects(MyTestApp::getSceneManagerS(), mTowerSceneNode);
		mTowerSceneNode->removeAndDestroyAllChildren();
        MyTestApp::getSceneManagerS()->destroySceneNode(mTowerSceneNode);
		mTowerSceneNode = 0;
	}
}

void Tower::create(Ogre::SceneNode* node)
{
	mTowerSceneNode = node->createChildSceneNode(mPosition);
	mTowerSceneNode->setUserAny(Ogre::Any(this));
	mTowerSceneNode->lookAt(mDirection, Ogre::Node::TransformSpace::TS_LOCAL);
	
    Ogre::Entity* bodyEntity = MyTestApp::getSceneManagerS()->createEntity(mBodyMesh);

	mTowerSceneNode->attachObject(bodyEntity);

	/*mTowerTopSceneNode = mTowerSceneNode->createChildSceneNode(mHeadDescription.mPosition);
    mTowerTopSceneNode->attachObject(MyTestApp::getSceneManagerS()->createEntity(mHeadDescription.mMeshFile));*/

	/*for(std::vector<WeaponDescription>::iterator it= mWeaponDescriptions.begin(); it != mWeaponDescriptions.end(); ++it)
	{
		Weapon* weapon = WeaponFactory::getSingleton().createWeapon(mTowerTopSceneNode, *it);
		mWeapons.push_back(weapon);
	}*/

	mWeaponHolder->create(mTowerSceneNode);

	// Инициализируем физику 

	mBody = Physics::getSingleton().createRigidBody(bodyEntity->getName());
	mBody->setStaticShape(mTowerSceneNode, mShape, 0.1f, 0.6f, mTowerSceneNode->_getDerivedPosition(), mTowerSceneNode->_getDerivedOrientation());	
	
}

void Tower::setPosition(const Ogre::Vector3& value)
{
	mPosition = value;
}

void Tower::setDirection(const Ogre::Vector3& value)
{
	mDirection = value;
}

void Tower::update(const Ogre::Real dTime)
{
	//mTowerTopSceneNode->rotate(Ogre::Vector3::UNIT_Y, Ogre::Degree(mHeadDescription.mRotateSpeed*dTime));
	
	/*for(std::vector<Weapon*>::iterator it= mWeapons.begin(); it != mWeapons.end(); ++it)
	{
		(*it)->update(dTime);
	}*/
}

}
