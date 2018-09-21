#include "PlayerInterface.h"

template<> RAT::PlayerInterface* Ogre::Singleton<RAT::PlayerInterface>::msSingleton = 0;

namespace RAT
{

PlayerInterface::PlayerInterface()
	: mGUISheet(0)
{	
}

PlayerInterface::~PlayerInterface()
{
	CEGUI::WindowManager::getSingleton().destroyWindow(mGUISheet);
	CEGUI::OgreRenderer::destroySystem();
}

void PlayerInterface::setUp(Ogre::Vector2& workspace)
{
	mRenderer = &CEGUI::OgreRenderer::bootstrapSystem();
    CEGUI::ImageManager::getSingleton().loadImageset("bin/TaharezLook.imageset");
    CEGUI::Font::setDefaultResourceGroup("Fonts");
    CEGUI::Scheme::setDefaultResourceGroup("Schemes");
    CEGUI::WidgetLookManager::setDefaultResourceGroup("LookNFeel");
    CEGUI::WindowManager::setDefaultResourceGroup("Layouts");
 
    CEGUI::SchemeManager::getSingleton().createFromFile("TaharezLook.scheme");
 
     CEGUI::System::getSingleton().getDefaultGUIContext().getMouseCursor().setDefaultImage("TaharezLook/MouseArrow");

	mGUISheet = CEGUI::WindowManager::getSingleton().createWindow("DefaultWindow", "GUISheet");
	
    CEGUI::System::getSingleton().getDefaultGUIContext().setRootWindow(mGUISheet);
}

CEGUI::Window* PlayerInterface::getRootWindow()
{
	return mGUISheet;
}

bool PlayerInterface::keyPressed( const OIS::KeyEvent &arg )
{
    CEGUI::System &sys = CEGUI::System::getSingleton();

    if (sys.injectKeyDown(arg.key) && sys.injectChar(arg.text))
		return true;

    return false;
}
 
bool PlayerInterface::keyReleased( const OIS::KeyEvent &arg )
{
    if(CEGUI::System::getSingleton().injectKeyUp(arg.key)) 
		return true;

    return false;
}
 
CEGUI::MouseButton  PlayerInterface::convertButton(OIS::MouseButtonID buttonID)
{
    switch (buttonID)
    {
    case OIS::MB_Left:
        return CEGUI::LeftButton;
        break;
 
    case OIS::MB_Right:
        return CEGUI::RightButton;
        break;
 
    case OIS::MB_Middle:
        return CEGUI::MiddleButton;
        break;
 
    default:
        return CEGUI::LeftButton;
        break;
    }
}
 
bool PlayerInterface::mouseMoved( const OIS::MouseEvent &arg )
{
    if(CEGUI::System::getSingleton().injectMouseMove(arg.state.X.rel, arg.state.Y.rel)) 
		return true;

    return false;
}
 
bool PlayerInterface::mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id )
{
    if(CEGUI::System::getSingleton().injectMouseButtonDown(convertButton(id))) 
		return true;

    return false;
}
 
bool PlayerInterface::mouseReleased( const OIS::MouseEvent &arg, OIS::MouseButtonID id )
{
    if(CEGUI::System::getSingleton().injectMouseButtonUp(convertButton(id))) 
		return true;

    return false;
}

}
