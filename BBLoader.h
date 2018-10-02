#ifndef BBLoader_H
#define BBLoader_H

#include <Ogre.h>
#include "tinyxml.h"
#include "BonusBuildingBase.h"

namespace RAT
{

class BBLoader
{
public:
	BBLoader();

	BonusBuildingBase* loadBB(const Ogre::String& fileName, Ogre::SceneManager* sceneMgr, const Ogre::String &groupName);

protected:

	void virtual parseBonus(TiXmlElement* xmlNode, BonusBuildingBase* bbBase);

};

}

#endif