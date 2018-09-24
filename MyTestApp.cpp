#include "MyTestApp.h"

template<> RAT::MyTestApp* Ogre::Singleton<RAT::MyTestApp>::msSingleton = 0;

namespace RAT
{

MyTestApp::MyTestApp() : OgreBites::ApplicationContext("OgreTutorialApp")
{

}

bool MyTestApp::keyPressed(const OgreBites::KeyboardEvent& evt)
{
    if (evt.keysym.sym == OgreBites::SDLK_ESCAPE)
    {
        getRoot()->queueEndRendering();
    }
    return true;
}

void MyTestApp::setup(void)
{
    // do not forget to call the base first
    OgreBites::ApplicationContext::setup();
    // get a pointer to the already created root
    root_ = getRoot();
    scnMgr_ = root_->createSceneManager();
    mWindow = getRenderWindow();
    // register our scene with the RTSS
    shadergen_ = Ogre::RTShader::ShaderGenerator::getSingletonPtr();
    shadergen_->addSceneManager(scnMgr_);
    // without light we would just get a black screen
    Ogre::Light* light = scnMgr_->createLight("MainLight");
    Ogre::SceneNode* lightNode = scnMgr_->getRootSceneNode()->createChildSceneNode();
    lightNode->setPosition(20, 80, 50);
    lightNode->attachObject(light);
    // also need to tell where we are
    Ogre::SceneNode* camNode = scnMgr_->getRootSceneNode()->createChildSceneNode();
    camNode->setPosition(0, 47, 222);
    camNode->lookAt(Ogre::Vector3(0, 0, -1), Ogre::Node::TS_PARENT);
    // create the camera
    Ogre::Camera* cam = scnMgr_->createCamera("myCam");
    cam->setNearClipDistance(5); // specific to this sample
    cam->setAutoAspectRatio(true);
    camNode->attachObject(cam);
    // and tell it to render into the main window
    getRenderWindow()->addViewport(cam);
    Ogre::TextureManager::getSingleton().setDefaultNumMipmaps(5);
    Ogre::Viewport* viewPort = getRenderWindow()->getViewport(0);
    viewPort->setBackgroundColour(Ogre::ColourValue(.0f, .0f, .0f));
    // finally something to render
    Ogre::Entity* ent = scnMgr_->createEntity("ogrehead.mesh");
    Ogre::Entity* ent2 = scnMgr_->createEntity("ogrehead.mesh");
    Ogre::SceneNode* node = scnMgr_->getRootSceneNode()->createChildSceneNode();
    Ogre::SceneNode* node2 = node->createChildSceneNode(Ogre::Vector3(84, 48, 0));
    node->attachObject(ent);
    node2->attachObject(ent2);
}

Ogre::SceneManager *MyTestApp::getSceneManager()
{
    return scnMgr_;
}

Ogre::RenderWindow *MyTestApp::getWindow()
{
    return MyTestApp::getSingleton().mWindow;
}

OIS::Keyboard *MyTestApp::getKeyboard()
{
    return mKeyboard;
}

}
