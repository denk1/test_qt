#include "GSMap.h"
#include "ITS.h"
#include "VehicleLoader.h"

namespace RAT
{

GSMap::GSMap(Map* map, ITS *app)
    : GameState(GameState::StMap), mMap(map), mVehicle(0), mIts(app)
{
    mInVehicle = true;
}

VehicleBase *GSMap::getVehicle()
{
    return mVehicle;
}

GSMap::~GSMap()
{
	if (mVehicle)
		delete mVehicle;

	delete mMap;
}

void GSMap::enteredState()
{

        VehiclePrototype* prot = VehicleLoader().loadVehicle("jeep.rtv", ITS::getSceneManagerS(), "Vehicles");
		mVehicle = new Vehicle(*prot);
		mVehicle->create(mMap->getMapSceneNode());
        mIts->setVehicle(mVehicle);
		delete prot;

		WeaponHolder* weapHolder = new WeaponHolder();
		HolderDescription desc;
		desc.mMeshFile = "jeep_cannon_body.mesh";
		desc.mPosition = Ogre::Vector3(0.f,0.f, 0.f);
	
		weapHolder->setDesctiption(desc);
		WeaponDescription weapDesc;
		weapDesc.mMeshFile = "jeep_cannon_gun.mesh";
		weapDesc.mType = "gun";
		weapDesc.mPosition = Ogre::Vector3(0,0,0);

		Weapon* weapon = WeaponFactory::getSingleton().buildWeapon(weapDesc);
		weapHolder->addWeapon(weapon);
		mVehicle->connectWeapon(0, weapHolder);

	// 

}


bool GSMap::frameStarted(const Ogre::FrameEvent& evt)
{
	if (mMap)
		mMap->frameStarted(evt);

	if (mVehicle)
		mVehicle->frameStarted(evt);

	return true && GameState::frameStarted(evt);
}

bool GSMap::frameRenderingQueued(const Ogre::FrameEvent& evt)
{
    Ogre::String statusText = "FPS: " + Ogre::StringConverter::toString(ITS::getSingleton().getWindow()->getStatistics().avgFPS);
	Ogre::Vector3 cam = TPCamera::getSingleton().getCamera()->getPosition();
	cam.x = (int)cam.x;cam.y = (int)cam.y;cam.z = (int)cam.z;
	statusText += "\nCameraPos: " + Ogre::StringConverter::toString(cam);
	cam = TPCamera::getSingleton().getCamera()->getDirection();
	statusText += "\nCameraDir: " + Ogre::StringConverter::toString(cam.x, 2) + " " + Ogre::StringConverter::toString(cam.y, 2) + " " + Ogre::StringConverter::toString(cam.z, 2);
	int unitsSpeed = mVehicle->getAvgSpeed();
	int kmhSpeed = mVehicle->getAvgSpeed(VehicleBase::Kmh);
    statusText += "\nSpeed(relative): " + Ogre::StringConverter::toString(unitsSpeed) + "(" + Ogre::StringConverter::toString(kmhSpeed) + " kmh)";
    // test
    //CEGUI::System::getSingleton().getDefaultGUIContext().getRootWindow()->getChild("Map/Framerate")->setText(statusText);
    // end test
    //CEGUI::WindowManager::getSingleton().getCh("Map/Framerate")->setText(statusText);


	if (mMap)
		mMap->frameRenderingQueued(evt);

	if (mVehicle)
		mVehicle->frameRenderingQueued(evt);

	return true && GameState::frameRenderingQueued(evt);
}

bool GSMap::frameEnded(const Ogre::FrameEvent& evt)
{
	if (mMap)
		mMap->frameEnded(evt);

	if (mVehicle)
		mVehicle->frameEnded(evt);

	return true && GameState::frameEnded(evt);
}


}
