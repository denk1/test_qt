#ifndef PlayerInterface_h
#define PlayerInterface_h

#include <OISEvents.h>
#include <OISInputManager.h>
#include <OISKeyboard.h>
#include <OISMouse.h>

#include <CEGUI/CEGUI.h>
#include <CEGUI/RendererModules/Ogre/Renderer.h>
#include <Ogre.h>
#include <OgreSingleton.h>
//#include "GameWindow.h"

namespace RAT
{

class PlayerInterface :  public Ogre::Singleton<PlayerInterface>, public OIS::KeyListener, public OIS::MouseListener
{
public:

	PlayerInterface(); 
	~PlayerInterface();
	
	// Инициализация
	void setUp(Ogre::Vector2& workspace);

	// Главное окно. Создается при старте и уничтажается при выходе из игры. 
	CEGUI::Window* getRootWindow();

protected:
	// OIS::KeyListener
    virtual bool keyPressed( const OIS::KeyEvent &arg );
    virtual bool keyReleased( const OIS::KeyEvent &arg );

    // OIS::MouseListener
    virtual bool mouseMoved( const OIS::MouseEvent &arg );
    virtual bool mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id );
    virtual bool mouseReleased( const OIS::MouseEvent &arg, OIS::MouseButtonID id );

	CEGUI::OgreRenderer*	mRenderer;
	CEGUI::Window*			mGUISheet;
	//GameWindow* mGameWindow;

private:
	CEGUI::MouseButton convertButton(OIS::MouseButtonID buttonID);
};

}

#endif
