#include "VehiclePrototype.h"

namespace RAT
{

VehiclePrototype::VehiclePrototype()
	: mShape()
	, mName("")
{
}

VehiclePrototype::~VehiclePrototype()
{
}

const VehicleProperties& VehiclePrototype::getProperties()
{
	return mProperties;
}

void VehiclePrototype::setProperties(const VehicleProperties& value)
{
	mProperties = value;
}

void VehiclePrototype::setMesh(const Ogre::String& value)
{
	mMesh = value;
}

void VehiclePrototype::setWheel(const int index, const VehicleWheel& wheel)
{
	if (index < 0 || index > 4)
		OGRE_EXCEPT(Ogre::Exception::ERR_INTERNAL_ERROR, "Wheel index out of range", "VehiclePrototype.setWheel");

	mWheels[index] = wheel;
}

void VehiclePrototype::setName(const Ogre::String& value)
{
	mName = value;
}

void VehiclePrototype::setResourceGroup(const Ogre::String& value)
{
	mResourceGroup = value;
}

VehiclePrototype::WCPType VehiclePrototype::wcpTypeFromString(const Ogre::String& value)
{
	if (!value.compare("heavy"))
		return WCHeavy;
	else if (!value.compare("medium"))
		return WCMedium;
	else if (!value.compare("light"))
		return WCLight;

	return WCLight;
}

void  VehiclePrototype::addConnectionPoint(const WCPoint& point)
{
	mConnectionPoints.push_back(point);
}

Ogre::String VehiclePrototype::getName()
{
	return mName;
}

void VehiclePrototype::setShape(OgreBulletCollisions::CollisionShape* shape)
{
	mShape = TShareShape(shape);
}

}
