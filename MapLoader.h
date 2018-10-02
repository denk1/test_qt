#ifndef MapLoader_H
#define MapLoader_H

#include <Ogre.h>
#include "Map.h"
#include "tinyxml.h"
#include "Landscape.h"
#include "Tower.h"

namespace RAT
{

class MapLoader
{
public:
	MapLoader();

	Map* loadMap(const Ogre::String& fileName, Ogre::SceneManager* sceneMgr, const Ogre::String &groupName);

protected:

	void virtual parseResource(TiXmlElement* xmlNode, Map* map, Ogre::SceneManager* sceneMgr);
	void virtual parseLight(TiXmlElement* xmlNode, Map* map, Ogre::SceneManager* sceneMgr);
	void virtual parseLandscape(TiXmlElement* xmlNode, Map* map, Ogre::SceneManager* sceneMgr);
	void virtual parseTower(TiXmlElement* xmlNode, Map* map, Ogre::SceneManager* sceneMgr);
	void virtual parseGameObjects(TiXmlElement* xmlNode, Map* map, Ogre::SceneManager* sceneMgr);
	void virtual parseBonusBuilding(TiXmlElement* xmlNode, Map* map, Ogre::SceneManager* sceneMgr);
	void virtual parseBonuses(TiXmlElement* xmlNode, Map* map, Ogre::SceneManager* sceneMgr);


	

};


}

#endif