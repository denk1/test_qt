#ifndef Map_H
#define Map_H

#include <Ogre.h>
#include <map>
#include "Landscape.h"
#include "Tower.h"
#include "BonusBuilding.h"
#include "GameObject.h"
#include "WeaponFactory.h"

namespace RAT
{

// Класс карты. Не подписывается на FrameListner (Его методы вызывает GSMap)
class Map : public Ogre::FrameListener
{
public:
	Map();
	virtual ~Map();
	

		// Задание ландшафта
	void setLandscape(Landscape* landscape);

		// Изменение света, возможно только до вызова Create()
	void setLightAmbient(const Ogre::ColourValue& value);
	void setLightDiffuse(const Ogre::ColourValue& value);
	void setLightSpecular(const Ogre::ColourValue& value);
	void setLightDirection(const Ogre::Vector3& value);

		// Имя файла сцены
	void setSceneFileName(const Ogre::String& value);

	void addTowerBase(TowerBase* towerBase);
	void addBBBase(BonusBuildingBase* bbBase);

	Tower* addTower(const Ogre::String& baseTowerName);
	BonusBuilding* addBonusBuilding(const Ogre::String& baseBBName);

		// Загрузить уровень
	void load();

	// Ogre::FrameListner
	bool frameRenderingQueued(const Ogre::FrameEvent& evt);

	Ogre::SceneNode* getMapSceneNode();

protected:
	
	Landscape* mLandscape;
	Ogre::SceneNode* mMapSceneNode;

	Ogre::Light* mLight;
	Ogre::ColourValue mLightAmbient;
	Ogre::ColourValue mLightDiffuse;
	Ogre::ColourValue mLightSpecular;
	Ogre::Vector3 mLightDirection;
	Ogre::String mSceneFileName;

	typedef std::map<Ogre::String, TowerBase*> TowerBaseMap;
	TowerBaseMap mTowerBases;

	typedef std::map<Ogre::String, BonusBuildingBase*> BBBMap;
	BBBMap mBBBases;
	

	std::vector<GameObject*> mGameObjects;
};

}

#endif