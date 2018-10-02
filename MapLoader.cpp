#include "MapLoader.h"
#include "TowerLoader.h"
#include "BBLoader.h"

namespace RAT
{

MapLoader::MapLoader()
{
}

Map* MapLoader::loadMap(const Ogre::String& fileName, Ogre::SceneManager* sceneMgr, const Ogre::String &groupName)
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
		Ogre::LogManager::getSingleton().logMessage("[MapLoader] Error creating TiXmlDocument");
        throw e;
    }
    catch(...)
    {
		if (xmlDoc)
			delete xmlDoc;
        OGRE_EXCEPT(Ogre::Exception::ERR_INTERNAL_ERROR, "Error creating TiXmlDocument", "MapLoader");
    }

	if( xmlDoc->Error() )
	{
		Ogre::String desc = Ogre::String(xmlDoc->ErrorDesc());
		delete xmlDoc;
		OGRE_EXCEPT(Ogre::Exception::ERR_INTERNAL_ERROR, "Xml error (" + desc + ")", "MapLoader");
	}

    xmlRoot = xmlDoc->RootElement();
    if( Ogre::String( xmlRoot->Value()) != "Map"  ) 
	{
        delete xmlDoc;      
		OGRE_EXCEPT(Ogre::Exception::ERR_INTERNAL_ERROR, "Error: Invalid map File. Missing <Map>", "MapLoader");
	}

		// Создаем карту
	Map* map = new Map();

	const char* attr = static_cast<TiXmlElement*>(xmlRoot)->Attribute("scene");
	if (attr)
		map->setSceneFileName(Ogre::String(attr));

		// Парсим используемые ресурсы
	TiXmlNode* node = xmlRoot->FirstChild("IncludeResources");
	if (node && (node->Type() == TiXmlNode::TINYXML_ELEMENT))
	{
		for(TiXmlNode* child = node->FirstChild(); child; child = child->NextSibling() )
		{
			if (child->Type() != TiXmlNode::NodeType::TINYXML_ELEMENT)
				continue;

			parseResource(static_cast<TiXmlElement*>(child), map, sceneMgr);
		}
	}
		// Парсим свет
	node = xmlRoot->FirstChild("Light");
	if (node && (node->Type() == TiXmlNode::TINYXML_ELEMENT))
	{
		parseLight(static_cast<TiXmlElement*>(node), map, sceneMgr);
	}

		// Парсим ландшафт
	node = xmlRoot->FirstChild("Landscape");
	if (node && (node->Type() == TiXmlNode::TINYXML_ELEMENT))
	{
		parseLandscape(static_cast<TiXmlElement*>(node), map, sceneMgr);
	}
		// Парсим игровые объекты
	node = xmlRoot->FirstChild("GameObjects");
	if (node && (node->Type() == TiXmlNode::TINYXML_ELEMENT))
	{
		parseGameObjects(static_cast<TiXmlElement*>(node), map, sceneMgr);
	}

	delete xmlDoc;

	return map;
}

void MapLoader::parseResource(TiXmlElement* xmlNode, Map* map, Ogre::SceneManager* sceneMgr)
{
	const char* attr = xmlNode->Attribute("type");
	if (!attr)
		return;
	Ogre::String type = Ogre::String(attr);

	attr = xmlNode->Attribute("src");
	if (!attr)
		return;
	Ogre::String src = Ogre::String(attr);

	attr = xmlNode->Attribute("resourceGroup");
	if (!attr)
		return;
	Ogre::String resourceGroup = Ogre::String(attr);

	if (!type.compare("Tower"))
	{
		TowerLoader loader;
		TowerBase* towerBase = loader.loadTower(src, sceneMgr, resourceGroup);
		map->addTowerBase(towerBase);
	}
	else if (!type.compare("BonusBuilding"))
	{
		BBLoader loader;
		BonusBuildingBase* bbBase = loader.loadBB(src, sceneMgr, resourceGroup);
		map->addBBBase(bbBase);
	}
}

void MapLoader::parseLight(TiXmlElement* xmlNode, Map* map, Ogre::SceneManager* sceneMgr)
{
	const char* attr;

	TiXmlNode* node = xmlNode->FirstChild("Ambient");
	Ogre::ColourValue color;
	
	if (node && (node->Type() == TiXmlNode::TINYXML_ELEMENT))
	{
		double value = 0;
		TiXmlElement* xmlElement = static_cast<TiXmlElement*>(node);
		attr = xmlElement->Attribute("r", &value);
		color.r = value;
		attr = xmlElement->Attribute("g", &value);
		color.g = value;
		attr = xmlElement->Attribute("b", &value);
		color.b = value;
		color.a = 1.0f;
		map->setLightAmbient(color);
	}

	node = xmlNode->FirstChild("Diffuse");
	
	if (node && (node->Type() == TiXmlNode::TINYXML_ELEMENT))
	{
		double value = 0;
		TiXmlElement* xmlElement = static_cast<TiXmlElement*>(node);
		attr = xmlElement->Attribute("r", &value);
		color.r = value;
		attr = xmlElement->Attribute("g", &value);
		color.g = value;
		attr = xmlElement->Attribute("b", &value);
		color.b = value;
		color.a = 1.0f;
		map->setLightDiffuse(color);
	}

	node = xmlNode->FirstChild("Specular");
	if (node && (node->Type() == TiXmlNode::TINYXML_ELEMENT))
	{
		double value = 0;
		TiXmlElement* xmlElement = static_cast<TiXmlElement*>(node);
		attr = xmlElement->Attribute("r", &value);
		color.r = value;
		attr = xmlElement->Attribute("g", &value);
		color.g = value;
		attr = xmlElement->Attribute("b", &value);
		color.b = value;
		color.a = 1.0f;
		map->setLightSpecular(color);
	}

	node = xmlNode->FirstChild("Direction");
	Ogre::Vector3 vec3;
	
	if (node && (node->Type() == TiXmlNode::TINYXML_ELEMENT))
	{
		double value = 0;
		TiXmlElement* xmlElement = static_cast<TiXmlElement*>(node);
		attr = xmlElement->Attribute("x", &value);
		vec3.x = value;
		attr = xmlElement->Attribute("y", &value);
		vec3.y = value;
		attr = xmlElement->Attribute("z", &value);
		vec3.z = value;

		map->setLightDirection(vec3);
	}
		
}

void MapLoader::parseLandscape(TiXmlElement* xmlNode, Map* map, Ogre::SceneManager* sceneMgr)
{
	LandscapeSettings settings;
	const char* attr;
	attr = xmlNode->Attribute("src");
	if (attr)
		settings.heightRawMap = Ogre::String(attr);

	attr = xmlNode->Attribute("resourceGroup");
	if (attr)
		settings.resourceGroup = Ogre::String(attr);

	xmlNode->Attribute("size", &settings.size);

	attr = xmlNode->Attribute("dat");
	if (attr)
		settings.compiledTerrain = Ogre::String(attr);
	double tmp = 0;
	xmlNode->Attribute("scaleXZ", &tmp);
	settings.scaleXZ = tmp;

	xmlNode->Attribute("scaleY", &tmp);
	settings.scaleY = tmp;

	Landscape* landscape = new Landscape();
	landscape->init(settings);

		// Парсим слои ландшафта
	TiXmlNode* node = xmlNode->FirstChild("Layers");
	for(TiXmlNode* child = node->FirstChild(); child; child = child->NextSibling() )
	{
		if (child->Type() != TiXmlNode::NodeType::TINYXML_ELEMENT)
			continue;

		TiXmlElement* xmlElement = static_cast<TiXmlElement*>(child);
		int worldSize = 30;
		xmlElement->Attribute("worldSize", &worldSize);

		landscape->addLayer(xmlElement->Attribute("diffuseSpecular"), xmlElement->Attribute("normalHeight"), xmlElement->Attribute("blend"), worldSize);
		
	}

	map->setLandscape(landscape);
}

void MapLoader::parseTower(TiXmlElement* xmlNode, Map* map, Ogre::SceneManager* sceneMgr)
{
	const char* attr = xmlNode->Attribute("type");
	if (!attr)
		return;

	Ogre::String type = attr;
	Tower* tower = map->addTower(type);
	if (!tower)
		return;

	double tmp = 0;

	TiXmlNode* node = xmlNode->FirstChild("Position");
	if (node && (node->Type() == TiXmlNode::TINYXML_ELEMENT))
	{
		Ogre::Vector3 pos;
		TiXmlElement* elNode = static_cast<TiXmlElement*>(node);
		elNode->Attribute("x", &tmp);
		pos.x = tmp;
		elNode->Attribute("y", &tmp);
		pos.y = tmp;
		elNode->Attribute("z", &tmp);
		pos.z = tmp;
		tower->setPosition(pos);
	}
	
	node = xmlNode->FirstChild("Direction");
	if (node && (node->Type() == TiXmlNode::TINYXML_ELEMENT))
	{
		Ogre::Vector3 dir;
		TiXmlElement* elNode = static_cast<TiXmlElement*>(node);
		elNode->Attribute("x", &tmp);
		dir.x = tmp;
		elNode->Attribute("y", &tmp);
		dir.y = tmp;
		elNode->Attribute("z", &tmp);
		dir.z = tmp;
		tower->setDirection(dir);
	}
}

void MapLoader::parseGameObjects(TiXmlElement* xmlNode, Map* map, Ogre::SceneManager* sceneMgr)
{
		// Парсим независимые башни уровня
	TiXmlNode* node = xmlNode->FirstChild("Towers");
	if (node)
		for(TiXmlNode* child = node->FirstChild(); child; child = child->NextSibling() )
		{
			if (child->Type() != TiXmlNode::NodeType::TINYXML_ELEMENT)
				continue;

			parseTower(static_cast<TiXmlElement*>(child), map, sceneMgr);
		}

		// Парсим бонусные здания
	node = xmlNode->FirstChild("BonusBuildings");
	if (node)
		for(TiXmlNode* child = node->FirstChild(); child; child = child->NextSibling() )
		{
			if (child->Type() != TiXmlNode::NodeType::TINYXML_ELEMENT)
				continue;

			parseBonusBuilding(static_cast<TiXmlElement*>(child), map, sceneMgr);
		}

		// Парсим бонусы
	node = xmlNode->FirstChild("Bonuses");
	if (node)
		for(TiXmlNode* child = node->FirstChild(); child; child = child->NextSibling() )
		{
			if (child->Type() != TiXmlNode::NodeType::TINYXML_ELEMENT)
				continue;

			parseBonuses(static_cast<TiXmlElement*>(child), map, sceneMgr);
		}
}

void MapLoader::parseBonusBuilding(TiXmlElement* xmlNode, Map* map, Ogre::SceneManager* sceneMgr)
{
	const char* attr = xmlNode->Attribute("type");
	if (!attr)
		return;

	Ogre::String type = Ogre::String(attr);
	BonusBuilding* bonusBuilding = map->addBonusBuilding(type);
	if (!bonusBuilding)
		return;

	double tmp = 0;

	TiXmlNode* node = xmlNode->FirstChild("Position");
	if (node && (node->Type() == TiXmlNode::TINYXML_ELEMENT))
	{
		Ogre::Vector3 pos;
		TiXmlElement* elNode = static_cast<TiXmlElement*>(node);
		elNode->Attribute("x", &tmp);
		pos.x = tmp;
		elNode->Attribute("y", &tmp);
		pos.y = tmp;
		elNode->Attribute("z", &tmp);
		pos.z = tmp;
		bonusBuilding->setPosition(pos);
	}
	
	node = xmlNode->FirstChild("Direction");
	if (node && (node->Type() == TiXmlNode::TINYXML_ELEMENT))
	{
		Ogre::Vector3 dir;
		TiXmlElement* elNode = static_cast<TiXmlElement*>(node);
		elNode->Attribute("x", &tmp);
		dir.x = tmp;
		elNode->Attribute("y", &tmp);
		dir.y = tmp;
		elNode->Attribute("z", &tmp);
		dir.z = tmp;
		bonusBuilding->setDirection(dir);
	}

		// Парсим зависимые башни
	return;
	node = xmlNode->FirstChild("Towers");
	if (node)
		for(TiXmlNode* child = node->FirstChild(); child; child = child->NextSibling() )
		{
			if (child->Type() != TiXmlNode::NodeType::TINYXML_ELEMENT)
				continue;

			parseTower(static_cast<TiXmlElement*>(child), map, sceneMgr);
		}
}

void MapLoader::parseBonuses(TiXmlElement* xmlNode, Map* map, Ogre::SceneManager* sceneMgr)
{
}

}
