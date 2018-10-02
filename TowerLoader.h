#ifndef TowerLoader_H
#define TowerLoader_H

#include <Ogre.h>
#include "tinyxml.h"
#include "TowerBase.h"

namespace RAT
{

class TowerLoader
{
public:
	TowerLoader();

	TowerBase* loadTower(const Ogre::String& fileName, Ogre::SceneManager* sceneMgr, const Ogre::String &groupName);

protected:

	void virtual parseBody(TiXmlElement* xmlNode, TowerBase* towerBase);
	void virtual parseHead(TiXmlElement* xmlNode, TowerBase* towerBase);

};

}

#endif