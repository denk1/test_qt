#include "Map.h"
#include "ITS.h"
#include "Physics.h"
#include "SceneLoader.h"

namespace RAT
{

Map::Map()
	:mLandscape(0)
	, mLight(0)
	, mSceneFileName("")
{
}

Map::~Map()
{
    ITS::getSceneManagerS()->destroyLight(mLight);
	mLight = 0;

	for(std::vector<GameObject*>::iterator it = mGameObjects.begin(); it != mGameObjects.end(); ++it)
	{
		delete *it;
	}

	for(TowerBaseMap::iterator it = mTowerBases.begin(); it != mTowerBases.end(); ++it)
		delete it->second;
	mTowerBases.clear();

	Physics::getSingleton().free();

	if (mLandscape)
	{
		delete mLandscape;
		mLandscape = 0;
	}

	if (mMapSceneNode)
	{
        ITS::destroyAllAttachedMovableObjects(ITS::getSceneManagerS(), mMapSceneNode);
		mMapSceneNode->removeAndDestroyAllChildren();
        ITS::getSceneManagerS()->destroySceneNode(mMapSceneNode);
		mMapSceneNode = 0;
	}
}

void Map::load()
{
    mMapSceneNode = ITS::getSceneManagerS()->getRootSceneNode()->createChildSceneNode("mapNode");

	if (mLandscape)
	{
		mLandscape->create();
		mLandscape->setupPhysics();
	}

    ITS::getSceneManagerS()->setAmbientLight(mLightAmbient);
	
    mLight = ITS::getSceneManagerS()->createLight();
	mLight->setDirection(mLightDirection);
	mLight->setDiffuseColour(mLightDiffuse);

	mLight->setSpecularColour(mLightSpecular);
	mLight->setType(Ogre::Light::LightTypes::LT_DIRECTIONAL);
	mLight->setCastShadows(true);

	for(std::vector<GameObject*>::iterator it = mGameObjects.begin(); it != mGameObjects.end(); ++it)
	{
		(*it)->create(mMapSceneNode);
	}

	if (mSceneFileName.compare(""))
	{
		SceneLoader loader;
        loader.parseDotScene(mSceneFileName, "General", ITS::getSingleton().getSceneManager(), mMapSceneNode);
	}

}

void Map::addTowerBase(TowerBase* towerBase)
{
    Ogre::Entity* bodyEntity = ITS::getSceneManagerS()->createEntity(towerBase->getBodyMesh());
	OgreBulletCollisions::CollisionShape* shape = Physics::getSingleton().createCollisionShapeFromMesh(bodyEntity);
	towerBase->setCollisionShape(shape);
	mTowerBases[towerBase->getName()] = towerBase;
}

void Map::addBBBase(BonusBuildingBase* bbBase)
{
    Ogre::Entity* bodyEntity = ITS::getSceneManagerS()->createEntity(bbBase->getBodyMesh());
	OgreBulletCollisions::CollisionShape* shape = Physics::getSingleton().createCollisionShapeFromMesh(bodyEntity);
	bbBase->setCollisionShape(shape);
	mBBBases[bbBase->getName()] = bbBase;
}

Tower* Map::addTower(const Ogre::String& baseTowerName)
{
	TowerBaseMap::iterator it = mTowerBases.find(baseTowerName);
	
	if (it != mTowerBases.end())
	{
		Tower* tower = new Tower(*it->second);
		mGameObjects.push_back(tower);
		return tower;
	}

	return 0;
}

BonusBuilding* Map::addBonusBuilding(const Ogre::String& baseBBName)
{
	BBBMap::iterator it = mBBBases.find(baseBBName);
	
	if (it != mBBBases.end())
	{
		BonusBuilding* bb = new BonusBuilding(*it->second);
		mGameObjects.push_back(bb);
		return bb;
	}

	return 0;
}


void Map::setLandscape(Landscape* landscape)
{
	mLandscape = landscape;
}

void Map::setLightAmbient(const Ogre::ColourValue& value)
{
	mLightAmbient = value;
}

void Map::setLightDiffuse(const Ogre::ColourValue& value)
{
	mLightDiffuse = value;
}

void Map::setLightSpecular(const Ogre::ColourValue& value)
{
	mLightSpecular = value;
}

void Map::setLightDirection(const Ogre::Vector3& value)
{
	mLightDirection = value;
}

void Map::setSceneFileName(const Ogre::String& value)
{
	mSceneFileName = value;
}

bool Map::frameRenderingQueued(const Ogre::FrameEvent& evt)
{
	for(std::vector<GameObject*>::iterator it = mGameObjects.begin(); it != mGameObjects.end(); ++it)
	{
		(*it)->update(evt.timeSinceLastFrame);
	}
	
	return true;
}

Ogre::SceneNode* Map::getMapSceneNode()
{
	return mMapSceneNode;
}

}
