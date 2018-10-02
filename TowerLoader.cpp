#include "TowerLoader.h"
#include "WeaponLoader.h"
#include "WeaponFactory.h"

namespace RAT
{

TowerLoader::TowerLoader()
{
}

TowerBase* TowerLoader::loadTower(const Ogre::String& fileName, Ogre::SceneManager* sceneMgr, const Ogre::String &groupName)
{
	TiXmlDocument* xmlDoc = 0;
    TiXmlElement* xmlRoot;

    try
    {
			// Открываем ресурс карты и грузим его в tinyxml
        Ogre::String basename, path;
        Ogre::StringUtil::splitFilename(fileName, basename, path);

        Ogre::DataStreamPtr pStream = Ogre::ResourceGroupManager::getSingleton().openResource( basename, groupName );

        Ogre::String data = pStream->getAsString();

        xmlDoc = new TiXmlDocument();
        xmlDoc->Parse( data.c_str() );
        pStream->close();
        pStream.setNull();

    }
	catch(Ogre::Exception& e)
    {
		if (xmlDoc)
			delete xmlDoc;
		Ogre::LogManager::getSingleton().logMessage("[TowerLoader] Error creating TiXmlDocument");
        throw e;
    }
    catch(...)
    {
		if (xmlDoc)
			delete xmlDoc;
        OGRE_EXCEPT(Ogre::Exception::ERR_INTERNAL_ERROR, "Error creating TiXmlDocument", "TowerLoader");
    }

	if( xmlDoc->Error() )
	{
		Ogre::String desc = Ogre::String(xmlDoc->ErrorDesc());
		delete xmlDoc;
		OGRE_EXCEPT(Ogre::Exception::ERR_INTERNAL_ERROR, "Xml error (" + desc + ")", "TowerLoader");
	}

    xmlRoot = xmlDoc->RootElement();
    if( Ogre::String( xmlRoot->Value()) != "Tower"  ) 
	{
        delete xmlDoc;      
		OGRE_EXCEPT(Ogre::Exception::ERR_INTERNAL_ERROR, "Error: Invalid map File. Missing <Tower>", "TowerLoader");
	}

		// Создаем прототип башни
	TowerBase* towerBase = new TowerBase();

	TiXmlElement* rootElement = static_cast<TiXmlElement*>(xmlRoot);

	towerBase->setBaseName(rootElement->Attribute("name"));

	towerBase->setResourceGroup(rootElement->Attribute("resourceGroup"));

	int tmp = 0;
	rootElement->Attribute("baseLife", &tmp);
	towerBase->setBaseLife(tmp);
	tmp = 0;

	rootElement->Attribute("respawnTime", &tmp);
	towerBase->setRespawnTime(tmp);
	tmp = 0;

	rootElement->Attribute("timeToUpgrade", &tmp);
	towerBase->setUpgradeTime(tmp);
	tmp = 0;

	
	TiXmlNode* node = xmlRoot->FirstChild("Body");
	if (node && (node->Type() == TiXmlNode::TINYXML_ELEMENT))
	{
		parseBody(static_cast<TiXmlElement*>(node), towerBase);
	}

	node = xmlRoot->FirstChild("Head");
	if (node && (node->Type() == TiXmlNode::TINYXML_ELEMENT))
	{
		parseHead(static_cast<TiXmlElement*>(node), towerBase);
	}

	delete xmlDoc;

	return towerBase;
}

void TowerLoader::parseBody(TiXmlElement* xmlNode, TowerBase* towerBase)
{
	const char* attr = xmlNode->Attribute("mesh");
	if (attr)
		towerBase->setBodyMesh(Ogre::String(attr));
}

void TowerLoader::parseHead(TiXmlElement* xmlNode, TowerBase* towerBase)
{
	HolderDescription desc;
	//
	const char* attr = xmlNode->Attribute("mesh");
	if (attr)
		desc.mMeshFile = Ogre::String(attr);
	double tmp = 0;
	xmlNode->Attribute("maxRotate", &tmp);
	desc.mMaxAngle = tmp;
	xmlNode->Attribute("rotateSpeed", &tmp);
	desc.mRotateSpeed = tmp;

	TiXmlNode* node = xmlNode->FirstChild("Position");
	if (node && (node->Type() == TiXmlNode::TINYXML_ELEMENT))
	{
		TiXmlElement* elNode = static_cast<TiXmlElement*>(node);
		elNode->Attribute("x", &tmp);
		desc.mPosition.x = tmp;
		elNode->Attribute("y", &tmp);
		desc.mPosition.y = tmp;
		elNode->Attribute("z", &tmp);
		desc.mPosition.z = tmp;
	}

	WeaponHolder* holder = new WeaponHolder();
	holder->setDesctiption(desc);

	towerBase->setWeaponHolder(holder);

	node = xmlNode->FirstChild("Weapons");
	if (node && (node->Type() == TiXmlNode::TINYXML_ELEMENT))
	{
		std::vector<WeaponDescription> descVec;
		WeaponLoader().parseWeapons(static_cast<TiXmlElement*>(node), descVec);
		for(std::vector<WeaponDescription>::iterator it = descVec.begin(); it != descVec.end(); it++)
		{
			Weapon* weapon = WeaponFactory::getSingleton().buildWeapon(*it);
			holder->addWeapon(weapon);

		}
	}

}

}
