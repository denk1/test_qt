#ifndef MY_TEST_APP_H
#define MY_TEST_APP_H
#include <iostream>
#include <OGRE/OgreBuildSettings.h>
#include <OGRE/Ogre.h>
#include <OGRE/OgreResourceManager.h>
#include <OGRE/Bites/OgreApplicationContext.h>
#include <OgreWindowEventUtilities.h>
#include "Physics.h"
#include "GSMap.h"
#include "PlayerInterface.h"
#include "GameState.h"
#include "TPCamera.h"
#include "VehicleBase.h"

namespace RAT
{

class TPCamera;
class GSMap;

class MyTestApp : public Ogre::Singleton<MyTestApp>, public OgreBites::ApplicationContext, public OgreBites::InputListener
{
public:
    MyTestApp();
    ~MyTestApp();
    bool keyPressed(const OgreBites::KeyboardEvent& evt);
    bool keyReleased(const OgreBites::KeyboardEvent& evt);
    void setup();
    Ogre::Root* getRoot();
    Ogre::SceneManager* getSceneManager();
    Ogre::RenderWindow* getWindow();
    const GameState* getGameState();

    static Ogre::Root* GetRootS();
    static GameState* getGameStateS();
    static Ogre::SceneManager* getSceneManagerS();
    static Ogre::RenderWindow* getWindowS();

    OIS::Keyboard* getKeyboard();
    OIS::Mouse* getMouse();

    void switchState(GameState* nextState);

    static void destroyAllAttachedMovableObjects(Ogre::SceneManager* sceneMgr, Ogre::SceneNode* sceneNode);
    void setVehicle(Vehicle *inVehicleBase);
protected:
    virtual void windowResized(Ogre::RenderWindow *rw);
    virtual void windowClosed(Ogre::RenderWindow *rw);

    //Ogre::FrameListener
    virtual bool frameRenderingQueued(const Ogre::FrameEvent &evt);
    bool isArg(const Ogre::String& arg);


private:
    std::vector<Ogre::String> mArgs;
    Ogre::SceneManager* scnMgr_;
    Ogre::Root* root_;
    Ogre::RenderWindow* mWindow;
    Ogre::RTShader::ShaderGenerator* shadergen_;
    Ogre::String mResourcesCfg_;
    GameState* mGameState;

    PlayerInterface* mPlayerInterface_;

    OIS::InputManager* mInputManager;
    OIS::Keyboard* mKeyboard;
    OIS::Mouse* mMouse;

    TPCamera* mCamera;
    Vehicle* mVehicle;
    GSMap* mGSMap;
    bool mInVehicle;

    bool mExit;


};
}
#endif
