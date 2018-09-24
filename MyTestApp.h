#ifndef MY_TEST_APP_H
#define MY_TEST_APP_H
#include <iostream>
#include <OGRE/OgreBuildSettings.h>
#include <OGRE/Ogre.h>
#include <OGRE/Bites/OgreApplicationContext.h>
#include "PlayerInterface.h"

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
    //const Ogre::GameState

    static Ogre::Root* GetRootS();

    OIS::Keyboard* getKeyboard();
    OIS::Mouse* getMouse();

    static Ogre::SceneManager* getSceneManagerS();
private:
    Ogre::SceneManager* scnMgr_;
    Ogre::Root* root_;
    Ogre::RenderWindow* mWindow;
    Ogre::RTShader::ShaderGenerator* shadergen_;

    OIS::Keyboard* mKeyboard;
    OIS::Mouse* mMouse;

};
}
#endif
