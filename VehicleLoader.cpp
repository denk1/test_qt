#include "VehicleLoader.h"

namespace RAT
{

VehicleLoader::VehicleLoader()
{
}

VehiclePrototype* VehicleLoader::loadVehicle(const Ogre::String& fileName, Ogre::SceneManager* sceneMgr, const Ogre::String &groupName)
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
		Ogre::LogManager::getSingleton().logMessage("[VehicleLoader] Error creating TiXmlDocument");
        throw e;
    }
    catch(...)
    {
		if (xmlDoc)
			delete xmlDoc;
        OGRE_EXCEPT(Ogre::Exception::ERR_INTERNAL_ERROR, "Error creating TiXmlDocument", "VehicleLoader");
    }

	if( xmlDoc->Error() )
	{
		Ogre::String desc = xmlDoc->ErrorDesc();
		delete xmlDoc;
		OGRE_EXCEPT(Ogre::Exception::ERR_INTERNAL_ERROR, "Xml error (" + desc + ")", "VehicleLoader");
	}

    xmlRoot = xmlDoc->RootElement();
    if( Ogre::String( xmlRoot->Value()) != "Vehicle"  ) 
	{
        delete xmlDoc;      
		OGRE_EXCEPT(Ogre::Exception::ERR_INTERNAL_ERROR, "Error: Invalid map File. Missing <Tower>", "VehicleLoader");
	}

		// Создаем прототип башни
	VehiclePrototype* vehiclePrototype = new VehiclePrototype();

	TiXmlElement* rootElement = static_cast<TiXmlElement*>(xmlRoot);

	vehiclePrototype->setName(rootElement->Attribute("name"));
	vehiclePrototype->setResourceGroup(rootElement->Attribute("resourceGroup"));
	vehiclePrototype->setMesh(rootElement->Attribute("mesh"));

	TiXmlNode* node = xmlRoot->FirstChild("Properties");
	if (node && (node->Type() == TiXmlNode::TINYXML_ELEMENT))
	{
		parseProperties(static_cast<TiXmlElement*>(node), vehiclePrototype);
	}

	node = xmlRoot->FirstChild("Wheels");
	if (node && (node->Type() == TiXmlNode::TINYXML_ELEMENT))
	{
		parseWheels(static_cast<TiXmlElement*>(node), vehiclePrototype);
	}

	node = xmlRoot->FirstChild("WeaponSlots");
	if (node && (node->Type() == TiXmlNode::TINYXML_ELEMENT))
	{
		parseWeaponSlots(static_cast<TiXmlElement*>(node), vehiclePrototype);
	}

	node = xmlRoot->FirstChild("Physics");
	if (node && (node->Type() == TiXmlNode::TINYXML_ELEMENT))
	{
		parsePhysics(static_cast<TiXmlElement*>(node), vehiclePrototype);
	}

	delete xmlDoc;

	return vehiclePrototype;
}

void VehicleLoader::parseProperties(TiXmlElement* xmlNode, VehiclePrototype* vehiclePrototype)
{
	VehicleProperties properties;
	TiXmlNode* node = xmlNode->FirstChild("Restitution");

	
	if (node && (node->Type() == TiXmlNode::TINYXML_ELEMENT))
		properties.mRestitution = Ogre::StringConverter::parseReal(static_cast<TiXmlElement*>(node)->GetText());

	node = xmlNode->FirstChild("Friction");
	if (node && (node->Type() == TiXmlNode::TINYXML_ELEMENT))
		properties.mFriction = Ogre::StringConverter::parseReal(static_cast<TiXmlElement*>(node)->GetText());

	node = xmlNode->FirstChild("Mass");
	if (node && (node->Type() == TiXmlNode::TINYXML_ELEMENT))
		properties.mMass = Ogre::StringConverter::parseReal(static_cast<TiXmlElement*>(node)->GetText());

	node = xmlNode->FirstChild("MaxEngineForce");
	if (node && (node->Type() == TiXmlNode::TINYXML_ELEMENT))
		properties.mMaxEngineForce = Ogre::StringConverter::parseReal(static_cast<TiXmlElement*>(node)->GetText());

	node = xmlNode->FirstChild("SteeringIncrement");
	if (node && (node->Type() == TiXmlNode::TINYXML_ELEMENT))
		properties.mSteeringIncrement = Ogre::StringConverter::parseReal(static_cast<TiXmlElement*>(node)->GetText());

	node = xmlNode->FirstChild("SteeringClamp");
	if (node && (node->Type() == TiXmlNode::TINYXML_ELEMENT))
		properties.mSteeringClamp= Ogre::StringConverter::parseReal(static_cast<TiXmlElement*>(node)->GetText());

	node = xmlNode->FirstChild("WheelRadius");
	if (node && (node->Type() == TiXmlNode::TINYXML_ELEMENT))
		properties.mWheelRadius = Ogre::StringConverter::parseReal(static_cast<TiXmlElement*>(node)->GetText());

	node = xmlNode->FirstChild("WheelFriction");
	if (node && (node->Type() == TiXmlNode::TINYXML_ELEMENT))
		properties.mWheelFriction = Ogre::StringConverter::parseReal(static_cast<TiXmlElement*>(node)->GetText());

	node = xmlNode->FirstChild("MaxSuspensionForce");
	if (node && (node->Type() == TiXmlNode::TINYXML_ELEMENT))
		properties.mMaxSuspensionForce = Ogre::StringConverter::parseReal(static_cast<TiXmlElement*>(node)->GetText());

	node = xmlNode->FirstChild("SuspensionStiffness");
	if (node && (node->Type() == TiXmlNode::TINYXML_ELEMENT))
		properties.mSuspensionStiffness = Ogre::StringConverter::parseReal(static_cast<TiXmlElement*>(node)->GetText());

	node = xmlNode->FirstChild("SuspensionDamping");
	if (node && (node->Type() == TiXmlNode::TINYXML_ELEMENT))
		properties.mSuspensionDamping = Ogre::StringConverter::parseReal(static_cast<TiXmlElement*>(node)->GetText());

	node = xmlNode->FirstChild("SuspensionCompression");
	if (node && (node->Type() == TiXmlNode::TINYXML_ELEMENT))
		properties.mSuspensionCompression = Ogre::StringConverter::parseReal(static_cast<TiXmlElement*>(node)->GetText());

	node = xmlNode->FirstChild("RollInfluence");
	if (node && (node->Type() == TiXmlNode::TINYXML_ELEMENT))
		properties.mRollInfluence = Ogre::StringConverter::parseReal(static_cast<TiXmlElement*>(node)->GetText());

	node = xmlNode->FirstChild("SuspensionRestLength");
	if (node && (node->Type() == TiXmlNode::TINYXML_ELEMENT))
		properties.mSuspensionRestLength= Ogre::StringConverter::parseReal(static_cast<TiXmlElement*>(node)->GetText());

	node = xmlNode->FirstChild("MaxSuspensionTravelCm");
	if (node && (node->Type() == TiXmlNode::TINYXML_ELEMENT))
		properties.mMaxSuspensionTravelCm = Ogre::StringConverter::parseReal(static_cast<TiXmlElement*>(node)->GetText());

	node = xmlNode->FirstChild("FrictionSlip");
	if (node && (node->Type() == TiXmlNode::TINYXML_ELEMENT))
		properties.mFrictionSlip = Ogre::StringConverter::parseReal(static_cast<TiXmlElement*>(node)->GetText());

	node = xmlNode->FirstChild("LinearDamping");
	if (node && (node->Type() == TiXmlNode::TINYXML_ELEMENT))
		properties.mLinearDamping = Ogre::StringConverter::parseReal(static_cast<TiXmlElement*>(node)->GetText());

	node = xmlNode->FirstChild("AngularDamping");
	if (node && (node->Type() == TiXmlNode::TINYXML_ELEMENT))
		properties.mAngularDamping = Ogre::StringConverter::parseReal(static_cast<TiXmlElement*>(node)->GetText());

	node = xmlNode->FirstChild("WeelOffsetW");
	if (node && (node->Type() == TiXmlNode::TINYXML_ELEMENT))
		properties.mWeelOffsetW = Ogre::StringConverter::parseReal(static_cast<TiXmlElement*>(node)->GetText());

	node = xmlNode->FirstChild("WeelOffsetL");
	if (node && (node->Type() == TiXmlNode::TINYXML_ELEMENT))
		properties.mWeelOffsetL = Ogre::StringConverter::parseReal(static_cast<TiXmlElement*>(node)->GetText());

	node = xmlNode->FirstChild("WeelOffsetH");
	if (node && (node->Type() == TiXmlNode::TINYXML_ELEMENT))
		properties.mWeelOffsetH = Ogre::StringConverter::parseReal(static_cast<TiXmlElement*>(node)->GetText());

	node = xmlNode->FirstChild("SpeedClampAffection");
	if (node && (node->Type() == TiXmlNode::TINYXML_ELEMENT))
		properties.mSpeedClampAffection = Ogre::StringConverter::parseReal(static_cast<TiXmlElement*>(node)->GetText());

	node = xmlNode->FirstChild("SpeedIncrementAffection");
	if (node && (node->Type() == TiXmlNode::TINYXML_ELEMENT))
		properties.mSpeedIncrementAffection = Ogre::StringConverter::parseReal(static_cast<TiXmlElement*>(node)->GetText());

	if (properties.mSpeedClampAffection < 0.001f)
		properties.mSpeedClampAffection = 1;

	if (properties.mSpeedIncrementAffection < 0.001f)
		properties.mSpeedIncrementAffection = 1;

	vehiclePrototype->setProperties(properties);
	
}

void VehicleLoader::parseWheels(TiXmlElement* xmlNode, VehiclePrototype* vehiclePrototype)
{
	Ogre::String wheelNames[] = {"FrontLeft", "FrontRight", "RearLeft", "ReartRight"};
	int indices[] = {VehiclePrototype::VFLWheelIndex, VehiclePrototype::VFRWheelIndex, VehiclePrototype::VRLWheelIndex, VehiclePrototype::VRRWheelIndex};
	for(int i = 0; i < 4; ++i)
	{
		TiXmlNode* node = xmlNode->FirstChild(wheelNames[i].c_str());
		if (node && (node->Type() == TiXmlNode::TINYXML_ELEMENT))
		{
			TiXmlElement* wheelNode = static_cast<TiXmlElement*>(node);
			VehicleWheel wheel;
			double value = 0;
			wheelNode->Attribute("engineForce", &value);
			wheel.mEngineForce = static_cast<float>(value);

			wheelNode->Attribute("engineBackForce", &value);
			wheel.mEngineBackForce = static_cast<float>(value);

			wheelNode->Attribute("breakForce", &value);
			wheel.mBreakForce = static_cast<float>(value);

			wheelNode->Attribute("handBreakForce", &value);
			wheel.mHandBreakForce = static_cast<float>(value);

			wheel.mMesh = wheelNode->Attribute("mesh");
			wheel.mHandBreakForce = static_cast<float>(value);

			vehiclePrototype->setWheel(indices[i], wheel);
			
		}
	}
}

void VehicleLoader::parseWeaponSlots(TiXmlElement* xmlNode, VehiclePrototype* vehiclePrototype)
{
	
	for(TiXmlNode* child = xmlNode->FirstChild(); child; child = child->NextSibling() )
	{
		if (child->Type() != TiXmlNode::NodeType::TINYXML_ELEMENT)
			continue;

		VehiclePrototype::WCPoint point;

		TiXmlElement* slot =  static_cast<TiXmlElement*>(child);
		Ogre::String type = slot->Attribute("class");
		point.mType = VehiclePrototype::wcpTypeFromString(type);
		if (!Ogre::String(slot->Attribute("static")).compare("no"))
			point.mIsStatic = false;
		else
			point.mIsStatic = true;

		TiXmlNode* node = slot->FirstChild("Position");
		if (node && (node->Type() == TiXmlNode::TINYXML_ELEMENT))
		{
			TiXmlElement* positionNode = static_cast<TiXmlElement*>(node);
			positionNode->QueryFloatAttribute("x", &point.mPosition.x);
			positionNode->QueryFloatAttribute("y", &point.mPosition.y);
			positionNode->QueryFloatAttribute("z", &point.mPosition.z);
		}

		node = slot->FirstChild("Rotation");
		if (node && (node->Type() == TiXmlNode::TINYXML_ELEMENT))
		{
			TiXmlElement* positionNode = static_cast<TiXmlElement*>(node);
			float yaw = 0;
			float pitch = 0;
			float roll = 0;
			positionNode->QueryFloatAttribute("yaw", &yaw);
			positionNode->QueryFloatAttribute("pitch", &pitch);
			positionNode->QueryFloatAttribute(" roll", & roll);
			Ogre::Matrix3 rotMat;
			rotMat.FromEulerAnglesXYZ(Ogre::Degree(yaw), Ogre::Degree(pitch), Ogre::Degree(roll));
			point.mRotation = Ogre::Quaternion(rotMat);
		}

		vehiclePrototype->addConnectionPoint(point);
	}
}

void VehicleLoader::parsePhysics(TiXmlElement* xmlNode, VehiclePrototype* vehiclePrototype)
{
	OgreBulletCollisions::CollisionShape* shape = PhysicsShapesLoader().parsePhysics(xmlNode);
	vehiclePrototype->setShape(shape);
}

}
