#include "PhysicsShapesLoader.h"
#include "tinyxml.h"

namespace RAT
{
PhysicsShapesLoader::PhysicsShapesLoader()
{
	
	mCollisionShapes["Sphere"]=&PhysicsShapesLoader::parseCollisionShapesSphere;
	mCollisionShapes["Box"]=&PhysicsShapesLoader::parseCollisionShapesBox;
	mCollisionShapes["Cone"]=&PhysicsShapesLoader::parseCollisionShapesCone;
	mCollisionShapes["Cylinder"]=&PhysicsShapesLoader::parseCollisionShapesCylinder;
	mCollisionShapes["Capsule"]=&PhysicsShapesLoader::parseCollisionShapesCapsule;
	mCollisionShapes["MultiSphere"]=&PhysicsShapesLoader::parseCollisionShapesMultiSphere;
	mCollisionShapes["Compound"]=&PhysicsShapesLoader::parseCollisionShapesCompound;
}

PhysicsShapesLoader::~PhysicsShapesLoader()
{

}
//получаем значение Ogre::Real
Ogre::Real PhysicsShapesLoader::getXmlRealAttribute(TiXmlElement* xmlNode, const char* attr)
{
	Ogre::Real myReal= Ogre::Real();
	if(xmlNode->Type()==TiXmlElement::NodeType::TINYXML_ELEMENT)
	{
		xmlNode->QueryFloatAttribute(attr, &myReal);
	}
	return myReal;
}
//получаем координаты базисного вектора 
void PhysicsShapesLoader::getXmlTagVectorAttribute(TiXmlElement* xmlNode, Ogre::Vector3& outVector )
{
	if(xmlNode->Type()==TiXmlElement::NodeType::TINYXML_ELEMENT)
	{
		if(xmlNode->QueryFloatAttribute("x", &outVector.x)!=TIXML_SUCCESS)
		{
			outVector.x=10;
		}
		if(xmlNode->QueryFloatAttribute("y", &outVector.y)!=TIXML_SUCCESS)
		{
			outVector.y=10;
		}
		if(xmlNode->QueryFloatAttribute("z", &outVector.z)!=TIXML_SUCCESS)
		{
			outVector.z=10;
		}
	}
}
//парсим коробку
OgreBulletCollisions::CollisionShape* PhysicsShapesLoader::parseCollisionShapesBox(TiXmlElement* xmlNode, Ogre::Vector3& position)
{
	Ogre::Vector3 vector3BoxBound;
	if( Ogre::String( xmlNode->Value()) != "Box" || xmlNode->NoChildren() || xmlNode->Type()!=TiXmlElement::NodeType::TINYXML_ELEMENT)
	{
		OGRE_EXCEPT(Ogre::Exception::ERR_INTERNAL_ERROR, "Error:Invalid physics definition. Missing tag <Box></Box> or error is its definition ", "PhysicsShapesLoader");
	}
	for(TiXmlElement* childXmlNode = xmlNode->FirstChildElement(); childXmlNode; childXmlNode=childXmlNode->NextSiblingElement())
	{
		if(Ogre::String(childXmlNode->Value())=="boxBound")
		{
			getXmlTagVectorAttribute(childXmlNode, vector3BoxBound);
		}
		else if(Ogre::String(childXmlNode->Value())=="Position")
		{
			getXmlTagVectorAttribute(childXmlNode, position);
		}
	}

	return Physics::getSingleton().createBoxCollisionShape(vector3BoxBound);
}
//парсим сферу
OgreBulletCollisions::CollisionShape* PhysicsShapesLoader::parseCollisionShapesSphere(TiXmlElement* xmlNode, Ogre::Vector3& position)
{
	Ogre::Real ogreRealRadius = Ogre::Real();
	if( Ogre::String( xmlNode->Value()) != "Sphere" || xmlNode->NoChildren() || xmlNode->Type()!=TiXmlElement::NodeType::TINYXML_ELEMENT)
	{
		OGRE_EXCEPT(Ogre::Exception::ERR_INTERNAL_ERROR, "Error:Invalid physics definition. Missing tag <Sphere></Sphere> or error is its definition ", "PhysicsShapesLoader");
	}
	for(TiXmlElement* childXmlNode = xmlNode->FirstChildElement(); childXmlNode; childXmlNode=childXmlNode->NextSiblingElement())
	{
		if(Ogre::String(childXmlNode->Value())=="Radius")
		{
			if(ogreRealRadius = getXmlRealAttribute(xmlNode, "value"))
			{
				OGRE_EXCEPT(Ogre::Exception::ERR_INTERNAL_ERROR, "Error: Into Vector3 is parsed or created ", "PhysicsShapesLoader");
			}
		}
		else if(Ogre::String(childXmlNode->Value())=="Position")
		{
			getXmlTagVectorAttribute(childXmlNode, position);
		}
	}
	return Physics::getSingleton().createSphereCollisionShape(ogreRealRadius);
}
//парсим капсулу
OgreBulletCollisions::CollisionShape* PhysicsShapesLoader::parseCollisionShapesCapsule(TiXmlElement* xmlNode, Ogre::Vector3& position)
{
	Ogre::Vector3 vector3Axe;
	Ogre::Real ogreRealRadius = Ogre::Real(), ogreRealHeight = Ogre::Real();
	if( Ogre::String( xmlNode->Value()) != "Capsule" || xmlNode->NoChildren() || xmlNode->Type()!=TiXmlElement::NodeType::TINYXML_ELEMENT)
	{
		OGRE_EXCEPT(Ogre::Exception::ERR_INTERNAL_ERROR, "Error:Invalid physics definition. Missing tag <Capsule></Capsule> or error is its definition ", "PhysicsShapesLoader");
	}
	for(TiXmlElement* childXmlNode = xmlNode->FirstChildElement(); childXmlNode; childXmlNode=childXmlNode->NextSiblingElement())
	{
		if(Ogre::String(childXmlNode->Value())=="Radius")
		{
			ogreRealRadius = getXmlRealAttribute(childXmlNode, "value");
		}
		else if(Ogre::String(childXmlNode->Value())=="Height")
		{
			ogreRealHeight = getXmlRealAttribute(childXmlNode, "value");
		}
		else if(Ogre::String(childXmlNode->Value())=="Axe")
		{
			getXmlTagVectorAttribute(childXmlNode, vector3Axe);
		}
		else if(Ogre::String(childXmlNode->Value())=="Position")
		{
			getXmlTagVectorAttribute(childXmlNode, position);
		}
	}
	return Physics::getSingleton().createCapsuleCollisionShape(ogreRealRadius, ogreRealHeight, vector3Axe);
}
//парсим конус
OgreBulletCollisions::CollisionShape* PhysicsShapesLoader::parseCollisionShapesCone(TiXmlElement* xmlNode, Ogre::Vector3& position)
{
	Ogre::Vector3 vector3Axe;
	Ogre::Real ogreRealRadius, ogreRealHeight;
	if( Ogre::String( xmlNode->Value()) != "Cone" || xmlNode->NoChildren() || xmlNode->Type()!=TiXmlElement::NodeType::TINYXML_ELEMENT)
	{
		OGRE_EXCEPT(Ogre::Exception::ERR_INTERNAL_ERROR, "Error:Invalid physics definition. Missing tag <Cone></Cone> or error is its definition ", "PhysicsShapesLoader");
	}
	for(TiXmlElement* childXmlNode = xmlNode->FirstChildElement(); childXmlNode; childXmlNode=childXmlNode->NextSiblingElement())
	{
		if(Ogre::String(childXmlNode->Value())=="Radius")
		{
			ogreRealRadius = getXmlRealAttribute(childXmlNode, "value");
		}
		else if(Ogre::String(childXmlNode->Value())=="Height")
		{
			ogreRealHeight = getXmlRealAttribute(childXmlNode, "value");
		}
		else if(Ogre::String(childXmlNode->Value())=="Axe")
		{
			getXmlTagVectorAttribute(childXmlNode, vector3Axe);
		}
		else if(Ogre::String(childXmlNode->Value())=="Position")
		{
			getXmlTagVectorAttribute(childXmlNode, position);
		}
	}
	return Physics::getSingleton().createConeCollisionShape(ogreRealRadius, ogreRealHeight, vector3Axe);
}
//парсим цилиндр 
OgreBulletCollisions::CollisionShape* PhysicsShapesLoader::parseCollisionShapesCylinder(TiXmlElement* xmlNode, Ogre::Vector3& position)
{
	Ogre::Vector3 vector3HalfExtents, vector3Axe; 
	if( Ogre::String( xmlNode->Value()) != "Cylinder" || xmlNode->NoChildren() || xmlNode->Type()!=TiXmlElement::NodeType::TINYXML_ELEMENT)
	{
		OGRE_EXCEPT(Ogre::Exception::ERR_INTERNAL_ERROR, "Error:Invalid physics definition. Missing tag <Cylinder></Cylinder> or error is its definition ", "PhysicsShapesLoader");
	}
	for(TiXmlElement* childXmlNode = xmlNode->FirstChildElement(); childXmlNode; childXmlNode = childXmlNode->NextSiblingElement())
	{
		if(Ogre::String(childXmlNode->Value())=="HalfExtents")
		{
			getXmlTagVectorAttribute(childXmlNode, vector3HalfExtents);
		}
		else if(Ogre::String(childXmlNode->Value())=="Axe")
		{
			getXmlTagVectorAttribute(childXmlNode, vector3Axe);
		}
		else if(Ogre::String(childXmlNode->Value())=="Position")
		{
			getXmlTagVectorAttribute(childXmlNode, position);
		}
	}
	return Physics::getSingleton().createCylinderCollisionShape(vector3HalfExtents, vector3Axe);
}
//Парсим мультисферу
OgreBulletCollisions::CollisionShape* PhysicsShapesLoader::parseCollisionShapesMultiSphere(TiXmlElement* xmlNode, Ogre::Vector3& position)
{
	int numSphere = int();
	Ogre::Real radi;
	Ogre::Vector3* posSphere = new Ogre::Vector3;
	Ogre::Vector3 vector3InertiaHalfExtents, vector3Positions, vector3Position;
	if( Ogre::String( xmlNode->Value()) != "MultiSphere" || xmlNode->NoChildren() || xmlNode->Type()!=TiXmlElement::NodeType::TINYXML_ELEMENT)
	{
		OGRE_EXCEPT(Ogre::Exception::ERR_INTERNAL_ERROR, "Error:Invalid physics definition. Missing tag <MultiSphere></MultiSphere> or error is its definition ", "PhysicsShapesLoader");
	}
	for(TiXmlElement* childXmlNode = xmlNode->FirstChildElement(); childXmlNode; childXmlNode = childXmlNode->NextSiblingElement())
	{
		if(Ogre::String(childXmlNode->Value())=="InertiaHalfExtents")
		{
			getXmlTagVectorAttribute(childXmlNode, vector3InertiaHalfExtents);
		}
		else if(Ogre::String(childXmlNode->Value())=="Positions")
		{
			for(TiXmlElement* childPosNode = childXmlNode->FirstChildElement(); childXmlNode; childXmlNode = childXmlNode->NextSiblingElement())
			{

				getXmlTagVectorAttribute(childPosNode, posSphere[numSphere++]);
			}
		}
		else if(Ogre::String(childXmlNode->Value())=="Radius")
		{
			radi = getXmlRealAttribute(childXmlNode, "value");
		}
		else if(Ogre::String(childXmlNode->Value())=="Position")
		{
			getXmlTagVectorAttribute(childXmlNode, position);
		}
	}
	return Physics::getSingleton().createMultiSphereCollisionShape(vector3InertiaHalfExtents, posSphere, &radi, numSphere); 
}
//Парсим компоуд
OgreBulletCollisions::CollisionShape* PhysicsShapesLoader::parseCollisionShapesCompound(TiXmlElement* xmlNode, Ogre::Vector3& position)
{
	OgreBulletCollisions::CompoundCollisionShape* shape = Physics::getSingleton().createCompoundCollisionShape();
	if( Ogre::String( xmlNode->Value()) != "Compound" || xmlNode->NoChildren() || xmlNode->Type()!=TiXmlElement::NodeType::TINYXML_ELEMENT)
	{
		OGRE_EXCEPT(Ogre::Exception::ERR_INTERNAL_ERROR, "Error:Invalid physics definition. Missing tag <Compound></Compound> or error is its definition ", "PhysicsShapesLoader");
	}
	for(TiXmlElement* childXmlNode = xmlNode->FirstChildElement(); childXmlNode; childXmlNode = childXmlNode->NextSiblingElement())
	{
		if(Ogre::String(childXmlNode->Value())=="Position")
		{
			getXmlTagVectorAttribute(childXmlNode, position);
		}
		else if(Ogre::String(childXmlNode->Value())=="Sphere" || Ogre::String(childXmlNode->Value())=="Box" || Ogre::String(childXmlNode->Value())=="Capsule" || Ogre::String(childXmlNode->Value())=="Cone" || Ogre::String(childXmlNode->Value())=="MultiSphere" || Ogre::String(childXmlNode->Value())=="Cylinder" || Ogre::String(childXmlNode->Value())=="Compound")
		{
			Ogre::Vector3 pos;
			OgreBulletCollisions::CollisionShape* childShape = (this->*mCollisionShapes.find(childXmlNode->Value())->second)(childXmlNode, pos);
			shape->addChildShape(childShape, pos);
		}
	}
	return shape;
}

OgreBulletCollisions::CompoundCollisionShape* PhysicsShapesLoader::getPhysics(TiXmlElement* xmlNode)
{
	Ogre::Vector3 position;
	OgreBulletCollisions::CompoundCollisionShape* compound = new OgreBulletCollisions::CompoundCollisionShape();
	for(TiXmlElement* childXmlNode = xmlNode->FirstChildElement(); childXmlNode; childXmlNode = childXmlNode->NextSiblingElement())
	{
		OgreBulletCollisions::CollisionShape* shape = (this->*mCollisionShapes.find(childXmlNode->Value())->second)(childXmlNode, position);
		compound->addChildShape(shape, position);
	}
	return compound;
}

OgreBulletCollisions::CompoundCollisionShape* PhysicsShapesLoader::parsePhysics(TiXmlElement* xmlNode)
{
	if( Ogre::String( xmlNode->Value()) != "Physics"  ) 
    {
        OGRE_EXCEPT(Ogre::Exception::ERR_INTERNAL_ERROR, "Error: Invalid physics definition. Missing ", "VehicleLoader");
    }
	return getPhysics(xmlNode);
}
}
