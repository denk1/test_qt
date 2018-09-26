#ifndef MY_TEST_APP_H
#define MY_TEST_APP_H
#include <iostream>
#include <OGRE/OgreBuildSettings.h>
#include <OGRE/Ogre.h>
#include <OGRE/OgreResourceManager.h>
#include <OGRE/Bites/OgreApplicationContext.h>
#include "PlayerInterface.h"
#include "GameState.h"

namespace RAT
{


class MyTestApp : public Ogre::Singleton<MyTestApp>, public OgreBites::ApplicationContext, public OgreBites::InputListener
{
public:
    MyTestApp();
    bool keyPressed(const OgreBites::KeyboardEvent& evt);
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
private:
    Ogre::SceneManager* scnMgr_;
    Ogre::Root* root_;
    Ogre::RenderWindow* mWindow;
    Ogre::RTShader::ShaderGenerator* shadergen_;
    Ogre::String mResourcesCfg_;
    GameState* mGameState;

    PlayerInterface* mPlayerInterface_;

    OIS::Keyboard* mKeyboard;
    OIS::Mouse* mMouse;

};
}
#endif
