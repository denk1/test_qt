#include "BBLoader.h"

namespace RAT
{

BBLoader::BBLoader()
{
}

BonusBuildingBase* BBLoader::loadBB(const Ogre::String& fileName, Ogre::SceneManager* sceneMgr, const Ogre::String &groupName)
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
		Ogre::LogManager::getSingleton().logMessage("[BBLoader] Error creating TiXmlDocument");
        throw e;
    }
    catch(...)
    {
		if (xmlDoc)
			delete xmlDoc;
        OGRE_EXCEPT(Ogre::Exception::ERR_INTERNAL_ERROR, "Error creating TiXmlDocument", "BBLoader");
    }

	if( xmlDoc->Error() )
	{
		Ogre::String desc = Ogre::String(xmlDoc->ErrorDesc());
		delete xmlDoc;
		OGRE_EXCEPT(Ogre::Exception::ERR_INTERNAL_ERROR, "Xml error (" + desc + ")", "BBLoader");
	}

    xmlRoot = xmlDoc->RootElement();
    if( Ogre::String( xmlRoot->Value()) != "BonusBulding"  ) 
	{
        delete xmlDoc;      
		OGRE_EXCEPT(Ogre::Exception::ERR_INTERNAL_ERROR, "Error: Invalid Bonus Bulding File. Missing <BonusBulding>", "BBLoader");
	}

		// Создаем прототип башни
	BonusBuildingBase* bbBase = new BonusBuildingBase();

	TiXmlElement* rootElement = static_cast<TiXmlElement*>(xmlRoot);

	const char* attr = rootElement->Attribute("name");
	if (attr)
		bbBase->setBaseName(Ogre::String(attr));

	attr = rootElement->Attribute("resourceGroup");
	if (attr)
		bbBase->setResourceGroup(Ogre::String(attr));

	attr = rootElement->Attribute("mesh");
	if (attr)
		bbBase->setBodyMesh(Ogre::String(attr));

	int tmp = 0;
	rootElement->Attribute("timeToCapture", &tmp);
	bbBase->setTimeToCapture(tmp);

	for(TiXmlNode* child = rootElement->FirstChild(); child; child = child->NextSibling() )
	{
		if (child->Type() != TiXmlNode::NodeType::TINYXML_ELEMENT)
			continue;

		parseBonus(static_cast<TiXmlElement*>(child), bbBase);
	}


	delete xmlDoc;

	return bbBase;
}

void BBLoader::parseBonus(TiXmlElement* xmlNode, BonusBuildingBase* bbBase)
{
	const char* attr = xmlNode->Attribute("type");
	Bonus bonus;
	if (attr)
		bonus.setType(Ogre::String(attr));
	int tmp = 0;
	xmlNode->Attribute("value", &tmp);
	bonus.setValue(tmp);
}

}
