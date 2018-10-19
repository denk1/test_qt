#include "GSMap.h"
#include "MyTestApp.h"
#include "VehicleLoader.h"

namespace RAT
{

GSMap::GSMap(Map* map, MyTestApp *app)
    : GameState(GameState::StMap), mMap(map), mIngameMenu(0), mVehicle(0), mApp(app)
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
	delete mIngameMenu;
    //CEGUI::WindowManager::getSingleton().destroyWindow("Map/Framerate");

	for(StateEventList::iterator it = mEvents.begin(); it != mEvents.end(); ++it)
		(*it)->disconnect();
	mEvents.clear();

    //CEGUI::WindowManager::getSingleton().destroyWindow("Map/Background");
    //CEGUI::System::getSingleton().setGUISheet(0);
    CEGUI::System::getSingleton().getDefaultGUIContext().setRootWindow(0);
}

void GSMap::enteredState()
{
//	CEGUI::Window* rootWindow= PlayerInterface::getSingleton().getRootWindow();
//	CEGUI::WindowManager &wmgr = CEGUI::WindowManager::getSingleton();
//	CEGUI::Window* window = wmgr.createWindow("DefaultWindow", "Map/Background");
	
//    //CEGUI::System::getSingleton().setGUISheet(window);
//    CEGUI::System::getSingleton().getDefaultGUIContext().setRootWindow(window);
//	mIngameMenu = new IngameMenu(window);
//	mIngameMenu->getWindow()->setModalState(false);
//	mIngameMenu->getWindow()->hide();

//	window->subscribeEvent(CEGUI::Window::EventKeyDown, CEGUI::Event::Subscriber(&GSMap::keyDown, this));
//	window->subscribeEvent(CEGUI::Window::EventKeyUp, CEGUI::Event::Subscriber(&GSMap::keyUp, this));
//	window->subscribeEvent(CEGUI::Window::EventMouseButtonDown, CEGUI::Event::Subscriber(&GSMap::mouseDown, this));
//	window->subscribeEvent(CEGUI::Window::EventMouseButtonUp, CEGUI::Event::Subscriber(&GSMap::mouseUp, this));
//	window->subscribeEvent(CEGUI::Window::EventMouseMove, CEGUI::Event::Subscriber(&GSMap::mouseMove, this));
//	mEvents.push_back(rootWindow->subscribeEvent(CEGUI::Window::EventKeyDown, CEGUI::Event::Subscriber(&GSMap::keyDown, this)));
//	mEvents.push_back(rootWindow->subscribeEvent(CEGUI::Window::EventKeyUp, CEGUI::Event::Subscriber(&GSMap::keyUp, this)));
//	mEvents.push_back(rootWindow->subscribeEvent(CEGUI::Window::EventMouseButtonDown, CEGUI::Event::Subscriber(&GSMap::mouseDown, this)));
//	mEvents.push_back(rootWindow->subscribeEvent(CEGUI::Window::EventMouseButtonUp, CEGUI::Event::Subscriber(&GSMap::mouseUp, this)));
//	mEvents.push_back(rootWindow->subscribeEvent(CEGUI::Window::EventMouseMove, CEGUI::Event::Subscriber(&GSMap::mouseMove, this)));

//	CEGUI::Window* statusInfo = CEGUI::WindowManager::getSingleton().createWindow("TaharezLook/StaticText", "Map/Framerate");
//	statusInfo->setPosition(CEGUI::UVector2(CEGUI::UDim(0.01f, 0), CEGUI::UDim(0.01f, 0)));
//    CEGUI::USize size;
//    size.d_height = CEGUI::UDim(0.3f, 0);
//    size.d_width = CEGUI::UDim(0.1f, 0);
//    statusInfo->setSize(size);
//    //statusInfo->setSize(CEGUI::UVector2(CEGUI::UDim(0.3f, 0), CEGUI::UDim(0.1f, 0)));
//	statusInfo->setText(Ogre::StringConverter::toString(0));
	
//	statusInfo->setProperty("BackgroundEnabled", "false");
//	statusInfo->setProperty("FrameEnabled", "false");

//    window->addChild(statusInfo);
//	window->activate();

	// tmp test code

        VehiclePrototype* prot = VehicleLoader().loadVehicle("jeep.rtv", MyTestApp::getSceneManagerS(), "Vehicles");
		mVehicle = new Vehicle(*prot);
		mVehicle->create(mMap->getMapSceneNode());
        mApp->setVehicle(mVehicle);
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


bool GSMap::keyDown(const CEGUI::EventArgs &e)
{
	CEGUI::KeyEventArgs* args = (CEGUI::KeyEventArgs*)&e;
	if (mInVehicle)
	{
		switch(args->scancode)
		{
			case CEGUI::Key::W:
				mVehicle->processControl(VehicleBase::VSForwardPressed);
				break;
			case CEGUI::Key::A:
				mVehicle->processControl(VehicleBase::VSLeftPressed);
				break;
			case CEGUI::Key::S:
				mVehicle->processControl(VehicleBase::VSBackwardPressed);
				break;
			case CEGUI::Key::D:
				mVehicle->processControl(VehicleBase::VSRightPressed);
				break;

			case CEGUI::Key::Space:
				mVehicle->processControl(VehicleBase::VSHandBreakPressed);
				break;

			case CEGUI::Key::Escape:
				mIngameMenu->getWindow()->setModalState(true);
				mIngameMenu->getWindow()->show();
				mIngameMenu->getWindow()->activate();
                CEGUI::System::getSingleton().getDefaultGUIContext().getMouseCursor().setPosition(CEGUI::Vector2<float>(100,100));
                CEGUI::System::getSingleton().getDefaultGUIContext().getMouseCursor().show();
                //CEGUI::MouseCursor::getSingleton().setPosition(CEGUI::Vector2(100,100));
                //CEGUI::MouseCursor::getSingleton().show();
				break;
		}
	}
	else
	{
		switch(args->scancode)
		{
			case CEGUI::Key::W:
				TPCamera::getSingleton().startGoingForward();
				break;
			case CEGUI::Key::A:
				TPCamera::getSingleton().startGoingLeft();
				break;
			case CEGUI::Key::S:
				TPCamera::getSingleton().startGoingBack();
				break;
			case CEGUI::Key::D:
				TPCamera::getSingleton().startGoingRight();
				break;

			case CEGUI::Key::Escape:
				mIngameMenu->getWindow()->setModalState(true);
				mIngameMenu->getWindow()->show();
				mIngameMenu->getWindow()->activate();

                CEGUI::System::getSingleton().getDefaultGUIContext().getMouseCursor().setPosition(CEGUI::Vector2<float>(100,100));
                CEGUI::System::getSingleton().getDefaultGUIContext().getMouseCursor().show();
				break;
		}
	}
	

	return true;
}

bool GSMap::keyUp(const CEGUI::EventArgs &e)
{
	CEGUI::KeyEventArgs* args = (CEGUI::KeyEventArgs*)&e;
	if (mInVehicle)
	{
		switch(args->scancode)
		{
			case CEGUI::Key::F1:
				TPCamera::getSingleton().setTargetVehicle(0);
				mInVehicle = false;
				break;
			case CEGUI::Key::W:
				mVehicle->processControl(VehicleBase::VSForwardReleased);
				break;
			case CEGUI::Key::A:
				mVehicle->processControl(VehicleBase::VSLeftReleased);
				break;
			case CEGUI::Key::S:
				mVehicle->processControl(VehicleBase::VSBackwardReleased);
				break;
			case CEGUI::Key::D:
				mVehicle->processControl(VehicleBase::VSRightReleased);
				break;

			case CEGUI::Key::Space:
				mVehicle->processControl(VehicleBase::VSHandBreakReleased);
				break;
		}
	}
	else
	{
			switch(args->scancode)
		{
			case CEGUI::Key::F1:
				TPCamera::getSingleton().setTargetVehicle(mVehicle->getVehicleSN(), 9);
				mInVehicle = true;
				break;

			case CEGUI::Key::W:
				TPCamera::getSingleton().stopGoingForward();
				break;
			case CEGUI::Key::A:
				TPCamera::getSingleton().stopGoingLeft();
				break;
			case CEGUI::Key::S:
				TPCamera::getSingleton().stopGoingBack();
				break;
			case CEGUI::Key::D:
				TPCamera::getSingleton().stopGoingRight();
				break;
		}
	}

	return true;
}

bool GSMap::mouseDown(const CEGUI::EventArgs &e)
{
	CEGUI::MouseEventArgs* args = (CEGUI::MouseEventArgs*)&e;
	
			

	Ogre::Entity *entity = 0;		
	OgreBulletCollisions::CollisionShape *sceneBoxShape = 0;		
	Ogre::SceneNode* node = 0;
	Ogre::Vector3 pos;
	Ogre::Vector3 size = Ogre::Vector3::ZERO;
	Ogre::Vector3 position = (TPCamera::getCameraS()->getPosition() + TPCamera::getCameraS()->getDirection().normalisedCopy() * 2);	
	
	Ogre::AxisAlignedBox boundingB;

	switch(args->button)
	{
		case CEGUI::MouseButton::LeftButton:
            entity = MyTestApp::getSceneManagerS()->createEntity("sphere.mesh");
		
			sceneBoxShape = Physics::getSingleton().createSphereCollisionShape(entity->getBoundingRadius()*0.5f/6);
            node = MyTestApp::getSceneManagerS()->getRootSceneNode()->createChildSceneNode();
			node->scale(0.5f, 0.5f, 0.5f);
			break;

		case CEGUI::MouseButton::RightButton:
            entity = MyTestApp::getSceneManagerS()->createEntity("cube.mesh");
			
			boundingB = entity->getBoundingBox();
			size = boundingB.getSize(); size /= 2.0f;
			size *= 0.96f; 
			size *= 0.05f;
			sceneBoxShape = Physics::getSingleton().createBoxCollisionShape(size);

            node = MyTestApp::getSceneManagerS()->getRootSceneNode()->createChildSceneNode();
			node->scale(0.05f, 0.05f, 0.05f);
			break;

		default:
			return true;
	}

	entity->setMaterialName("Examples/BumpyMetal");
	node->attachObject(entity);
	//entity->setCastShadows(false);	
	
	OgreBulletDynamics::RigidBody *defaultBody = Physics::getSingleton().createRigidBody ();
	defaultBody->setShape(   node,
					sceneBoxShape,
					0.2f,         // dynamic body restitution
					0.5f,         // dynamic body friction
					300.0f,          // dynamic bodymass
					position,      // starting position of the box
					Ogre::Quaternion(0,0,0,1));// orientation of the box

			          
	defaultBody->setLinearVelocity(
			TPCamera::getCameraS()->getDerivedDirection().normalisedCopy() * 30.0f ); // shooting speed

	return true;
}
bool GSMap::mouseUp(const CEGUI::EventArgs &e)
{
	return true;
}
bool GSMap::mouseMove(const CEGUI::EventArgs &e)
{
	CEGUI::MouseEventArgs* args = (CEGUI::MouseEventArgs*)&e;
	
	
	TPCamera::getSingleton().rotate(args->moveDelta.d_x, args->moveDelta.d_y);

	return true;
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
    Ogre::String statusText = "FPS: " + Ogre::StringConverter::toString(MyTestApp::getSingleton().getWindow()->getStatistics().avgFPS);
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
