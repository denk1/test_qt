#include "MyTestApp.h"
#include "GSStart.h"
#include "MapLoader.h"
#include "WeaponFactory.h"
#include "Config.h"

template<> RAT::MyTestApp* Ogre::Singleton<RAT::MyTestApp>::msSingleton = 0;

namespace RAT
{

MyTestApp::MyTestApp() : OgreBites::ApplicationContext("OgreTutorialApp"), mInVehicle(true)
{
    new WeaponFactory();
    new Config();
}

MyTestApp::~MyTestApp()
{

}

bool MyTestApp::keyPressed(const OgreBites::KeyboardEvent& evt)
{
    if (evt.keysym.sym == OgreBites::SDLK_ESCAPE)
    {
        getRoot()->queueEndRendering();
        return true;
    }
    else if(evt.keysym.sym == OgreBites::SDLK_F2)
    {
        mCamera->setTargetFollowing();
    }

    if(mInVehicle)
    {

        if(evt.keysym.sym == 'w')
        {
            mVehicle->processControl(VehicleBase::VSForwardPressed);
            mCamera->changeSignDirection(-1);
        }
        else if(evt.keysym.sym == 's')
        {
            mVehicle->processControl(VehicleBase::VSBackwardPressed);
            mCamera->changeSignDirection(1);
        }
        else if(evt.keysym.sym == 'a')
        {
            mVehicle->processControl(VehicleBase::VSLeftPressed);
        }
        else if(evt.keysym.sym == 'd')
        {
            mVehicle->processControl(VehicleBase::VSRightPressed);
        }
    }
    else
    {
        if (evt.keysym.sym == 'w')
        {
            TPCamera::getSingleton().startGoingForward();
        }
        else if(evt.keysym.sym == 'a')
        {
            TPCamera::getSingleton().startGoingLeft();
        }
        else if(evt.keysym.sym == 's')
        {
            TPCamera::getSingleton().startGoingBack();
        }
        else if(evt.keysym.sym == 'd')
        {
            TPCamera::getSingleton().startGoingRight();
        }

    }
    return true;
}

bool MyTestApp::keyReleased(const OgreBites::KeyboardEvent &evt)
{
    if(mInVehicle)
    {
        if(evt.keysym.sym == OgreBites::SDLK_F1)
        {
            TPCamera::getSingleton().setTargetVehicle(mVehicle->getVehicleSN(), 0);
            mInVehicle = false;
        }
        else if(evt.keysym.sym == 'w')
        {
            mVehicle->processControl(VehicleBase::VSForwardReleased);
        }
        else if(evt.keysym.sym == 's')
        {
            mVehicle->processControl(VehicleBase::VSBackwardReleased);
        }
        else if(evt.keysym.sym == 'a')
        {
            mVehicle->processControl(VehicleBase::VSLeftReleased);
        }
        else if(evt.keysym.sym == 'd')
        {
            mVehicle->processControl(VehicleBase::VSRightReleased);
        }
    }
    else
    {
        if(evt.keysym.sym == OgreBites::SDLK_F1)
        {
            TPCamera::getSingleton().setTargetVehicle(mVehicle->getVehicleSN(), 9);
            mInVehicle = true;

        }
        else if(evt.keysym.sym == 'w')
        {
            TPCamera::getSingleton().stopGoingForward();
        }
        else if(evt.keysym.sym == 'a')
        {
            TPCamera::getSingleton().stopGoingLeft();
        }
        else if(evt.keysym.sym == 's')
        {
            TPCamera::getSingleton().stopGoingBack();
        }
        else if(evt.keysym.sym == 'd')
        {
            TPCamera::getSingleton().stopGoingRight();
        }
    }
    return true;
}

bool MyTestApp::mouseMoved(const OgreBites::MouseMotionEvent& evt)
{

    TPCamera::getSingleton().rotate(evt.xrel, evt.yrel);
    return true;
}



void MyTestApp::setup(void)
{
    root_ = getRoot();
    mResourcesCfg_ = "resources.cfg";
    // do not forget to call the base first
    OgreBites::ApplicationContext::setup();
    addInputListener(this);
    Ogre::ConfigFile cf;
    cf.load(mResourcesCfg_);
    Ogre::ConfigFile::SectionIterator seci = cf.getSectionIterator();
    Ogre::String secName, typeName, archName;
    while (seci.hasMoreElements())
    {
        secName = seci.peekNextKey();
        Ogre::ConfigFile::SettingsMultiMap *settings = seci.getNext();
        Ogre::ConfigFile::SettingsMultiMap::iterator i;
        for (i = settings->begin(); i != settings->end(); ++i)
        {
            typeName = i->first;
            archName = i->second;
            Ogre::ResourceGroupManager::getSingleton().addResourceLocation(
                archName, typeName, secName);
        }
    }
    // get a pointer to the already created root
    // Initialise the resource groups:
    Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
    scnMgr_ = root_->createSceneManager(Ogre::ST_EXTERIOR_FAR, "BulletTerrain");
//    scnMgr_->setSkyBox(true, "Cratelake");
    mWindow = getRenderWindow();
    // register our scene with the RTSS
    shadergen_ = Ogre::RTShader::ShaderGenerator::getSingletonPtr();
    shadergen_->addSceneManager(scnMgr_);
    // without light we would just get a black screen

    // create the camera
    mCamera = new TPCamera();
    Ogre::Camera* cam = mCamera->getCamera();
    Ogre::Viewport* viewPort = mWindow->addViewport(cam);
    //Ogre::Viewport* viewPort = getRenderWindow()->getViewport(0);
    viewPort->setBackgroundColour(Ogre::ColourValue(.0f, .0f, .0f));
    //root_->addFrameListener(mCamera);
    mRoot->addFrameListener(mCamera);
    //cam->setNearClipDistance(5); // specific to this sample
    cam->setAutoAspectRatio(true);
    Ogre::TextureManager::getSingleton().setDefaultNumMipmaps(5);
    windowResized(mWindow);
    scnMgr_->setShadowColour(Ogre::ColourValue(0.7, 0.7, 0.7));
    Ogre::AxisAlignedBox bounds = Ogre::AxisAlignedBox(
                Ogre::Vector3(-1000, -1000, -1000),
                Ogre::Vector3(1000, 1000, 1000)
                );
    //new Physics(bounds, true);
    new Physics(bounds);
    mGameState = new GSStart();
    MapLoader loader;
    Map* map;
    map = loader.loadMap("lowland.rtm", MyTestApp::getSingleton().getSceneManager(), "Maps");
    map->load();
    mGSMap = new GSMap(map, this);
    MyTestApp::getSingleton().switchState(mGSMap);
}

Ogre::Root *MyTestApp::getRoot()
{
    return mRoot;
}

Ogre::SceneManager *MyTestApp::getSceneManager()
{
    return scnMgr_;
}

Ogre::RenderWindow *MyTestApp::getWindow()
{
    return mWindow;
}

const GameState *MyTestApp::getGameState()
{
    return mGameState;
}

Ogre::Root *MyTestApp::GetRootS()
{
    return MyTestApp::getSingleton().mRoot;
}

GameState *MyTestApp::getGameStateS()
{
    return MyTestApp::getSingleton().mGameState;
}

OIS::Keyboard *MyTestApp::getKeyboard()
{
    return mKeyboard;
}

OIS::Mouse *MyTestApp::getMouse()
{
    return mMouse;
}

void MyTestApp::switchState(GameState *nextState)
{
    mGameState->switchState(nextState);
    mGameState = nextState;
}

void MyTestApp::destroyAllAttachedMovableObjects(Ogre::SceneManager *sceneMgr, Ogre::SceneNode *sceneNode)
{
    if(!sceneNode)
    {
        return;
    }

    Ogre::SceneNode::ObjectIterator itObject = sceneNode->getAttachedObjectIterator();

    for(int i = 0; i < sceneNode->numAttachedObjects(); ++i)
    {
        if(sceneNode->getAttachedObject(i)->getTypeFlags() != 4294967295)
            sceneMgr->destroyMovableObject(sceneNode->getAttachedObject(i));
        else
            return;
    }

    Ogre::SceneNode::ChildNodeIterator itChild = sceneNode->getChildIterator();

    while(itChild.hasMoreElements())
    {
        Ogre::SceneNode* childNode = static_cast<Ogre::SceneNode*>(itChild.getNext());
        destroyAllAttachedMovableObjects(sceneMgr, childNode);
    }
}

void MyTestApp::setVehicle(Vehicle *inVehicleBase)
{
    mVehicle = inVehicleBase;
}

void MyTestApp::windowResized(Ogre::RenderWindow *rw)
{
    unsigned int width, height, depth;
    int left, top;
    rw->getMetrics(width, height, depth, left, top);
}

void MyTestApp::windowClosed(Ogre::RenderWindow *rw)
{
    if(rw == mWindow)
    {
        if(mInputManager)
        {
            mInputManager->destroyInputObject( mMouse);
            mInputManager->destroyInputObject( mKeyboard);

            OIS::InputManager::destroyInputSystem(mInputManager);
            mInputManager = 0;
        }
    }
}

bool MyTestApp::frameRenderingQueued(const Ogre::FrameEvent &evt)
{
    if(mWindow->isClosed() || mExit)
        return false;

    //mKeyboard->capture();
    //mMouse->capture();

    return true;
}

bool MyTestApp::isArg(const Ogre::String &arg)
{
    for(std::vector<Ogre::String>::iterator it = mArgs.begin(); it != mArgs.end(); ++it)
        if (!it->compare(arg))
            return true;
    return false;
}

Ogre::SceneManager *MyTestApp::getSceneManagerS()
{
    return MyTestApp::getSingleton().scnMgr_;
}

Ogre::RenderWindow *MyTestApp::getWindowS()
{
    return MyTestApp::getSingleton().mWindow;
}

}
