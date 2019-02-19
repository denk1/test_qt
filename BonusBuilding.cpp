#include "BonusBuilding.h"
#include "ITS.h"

namespace RAT
{

BonusBuilding::BonusBuilding(const BonusBuildingBase& base)
	:BonusBuildingBase(base), GameObject(GameObject::BonusBuilding)
{

}

BonusBuilding::~BonusBuilding()
{
	Physics::getSingleton().destroyRigidBody(mBody);

    ITS::destroyAllAttachedMovableObjects(ITS::getSceneManagerS(), mSceneNode);
	mSceneNode->removeAndDestroyAllChildren();
    ITS::getSceneManagerS()->destroySceneNode(mSceneNode);
}

void BonusBuilding::create(Ogre::SceneNode* node)
{
	mSceneNode = node->createChildSceneNode(mPosition);
	mSceneNode->setUserAny(Ogre::Any(this));
	mSceneNode->lookAt(mDirection, Ogre::Node::TransformSpace::TS_LOCAL);
	
    Ogre::Entity* bodyEntity = ITS::getSceneManagerS()->createEntity(mBodyMesh);

	mSceneNode->attachObject(bodyEntity);

	// Инициализируем физику 

	mBody = Physics::getSingleton().createRigidBody(bodyEntity->getName());
	mBody->setStaticShape(mSceneNode, mShape, 0.1f, 0.6f, mSceneNode->_getDerivedPosition(), mSceneNode->_getDerivedOrientation());	
	
}

void BonusBuilding::setPosition(const Ogre::Vector3& value)
{
	mPosition = value;
}

void BonusBuilding::setDirection(const Ogre::Vector3& value)
{
	mDirection = value;
}

void BonusBuilding::update(const Ogre::Real dTime)
{
	// pass
}

}
