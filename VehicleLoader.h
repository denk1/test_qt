#ifndef VehicleLoader_H
#define VehicleLoader_H

#include <Ogre.h>
#include "tinyxml.h"
#include "VehiclePrototype.h"
#include "PhysicsShapesLoader.h"

namespace RAT
{

class VehicleLoader
{
public:
	VehicleLoader();

	VehiclePrototype* loadVehicle(const Ogre::String& fileName, Ogre::SceneManager* sceneMgr, const Ogre::String &groupName);

protected:

	void virtual parseProperties(TiXmlElement* xmlNode, VehiclePrototype* vehiclePrototype);
	void virtual parseWheels(TiXmlElement* xmlNode, VehiclePrototype* vehiclePrototype);
	void virtual parseWeaponSlots(TiXmlElement* xmlNode, VehiclePrototype* vehiclePrototype);
	void virtual parsePhysics(TiXmlElement* xmlNode, VehiclePrototype* vehiclePrototype);

};

}

#endif