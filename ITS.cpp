#include "ITS.h"
#include "GSStart.h"
#include "MapLoader.h"
#include "WeaponFactory.h"
#include "Config.h"

template<> RAT::ITS* Ogre::Singleton<RAT::ITS>::msSingleton = 0;

namespace RAT
{

ITS::ITS() : OgreBites::ApplicationContext("OgreTutorialApp"), mInVehicle(true)
{
    new WeaponFactory();
    new Config();
}

ITS::~ITS()
{

}

bool ITS::keyPressed(const OgreBites::KeyboardEvent& evt)
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

bool ITS::keyReleased(const OgreBites::KeyboardEvent &evt)
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

bool ITS::mouseMoved(const OgreBites::MouseMotionEvent& evt)
{

    TPCamera::getSingleton().rotate(evt.xrel, evt.yrel);
    return true;
}



void ITS::setup(void)
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
    map = loader.loadMap("lowland.rtm", ITS::getSingleton().getSceneManager(), "Maps");
    map->load();
    mGSMap = new GSMap(map, this);
    ITS::getSingleton().switchState(mGSMap);
}

Ogre::Root *ITS::getRoot()
{
    return mRoot;
}

Ogre::SceneManager *ITS::getSceneManager()
{
    return scnMgr_;
}

Ogre::RenderWindow *ITS::getWindow()
{
    return mWindow;
}

const GameState *ITS::getGameState()
{
    return mGameState;
}

Ogre::Root *ITS::GetRootS()
{
    return ITS::getSingleton().mRoot;
}

GameState *ITS::getGameStateS()
{
    return ITS::getSingleton().mGameState;
}

void ITS::switchState(GameState *nextState)
{
    mGameState->switchState(nextState);
    mGameState = nextState;
}

void ITS::destroyAllAttachedMovableObjects(Ogre::SceneManager *sceneMgr, Ogre::SceneNode *sceneNode)
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

void ITS::setVehicle(Vehicle *inVehicleBase)
{
    mVehicle = inVehicleBase;
}

void ITS::windowResized(Ogre::RenderWindow *rw)
{
    unsigned int width, height, depth;
    int left, top;
    rw->getMetrics(width, height, depth, left, top);
}

void ITS::windowClosed(Ogre::RenderWindow *rw)
{
    if(rw == mWindow)
    {

    }
}

bool ITS::frameRenderingQueued(const Ogre::FrameEvent &evt)
{
    if(mWindow->isClosed() || mExit)
        return false;

    //mKeyboard->capture();
    //mMouse->capture();

    return true;
}

bool ITS::isArg(const Ogre::String &arg)
{
    for(std::vector<Ogre::String>::iterator it = mArgs.begin(); it != mArgs.end(); ++it)
        if (!it->compare(arg))
            return true;
    return false;
}

Ogre::SceneManager *ITS::getSceneManagerS()
{
    return ITS::getSingleton().scnMgr_;
}

Ogre::RenderWindow *ITS::getWindowS()
{
    return ITS::getSingleton().mWindow;
}

}
