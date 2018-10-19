#include "MainMenu.h"
#include "MyTestApp.h"
#include "MapLoader.h"
#include "GSMap.h"
#include "ConfigWindow.h"

namespace RAT
{

MainMenu::MainMenu(CEGUI::Window* parent)
{
    mWindow = CEGUI::WindowManager::getSingleton().loadLayoutFromFile("MainMenu.wnd");
    parent->addChild(mWindow);

	try
	{
		CEGUI::Window* buttonStart = mWindow->getChild("MainMenu/ButtonStart");
		buttonStart->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&MainMenu::buttonStartPressed, this));
	}
	catch(CEGUI::Exception& e)
	{
		// TODO: Add log message
	}
	try
	{
		CEGUI::Window* buttonConfig = mWindow->getChild("MainMenu/ButtonConfig");
		buttonConfig->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&MainMenu::buttonConfigPressed, this));
	}
	catch(CEGUI::Exception& e)
	{
		// TODO: Add log message
	}
	try
	{
		CEGUI::Window* buttonExit = mWindow->getChild("MainMenu/ButtonExit");
		buttonExit->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&MainMenu::buttonExitPressed, this));
	}
	catch(CEGUI::Exception& e)
	{
		// TODO: Add log message
	}
	try
	{
		CEGUI::Window* buttonConsole = mWindow->getChild("MainMenu/ButtonConsole");
		buttonConsole->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&MainMenu::buttonConsolePressed, this));
	}
	catch(CEGUI::Exception& e)
	{

	}
}


bool MainMenu::buttonStartPressed(const CEGUI::EventArgs &e)
{
	mWindow->setModalState(false);
	mWindow->hide();
    CEGUI::System::getSingleton().getDefaultGUIContext().getMouseCursor().hide();

    //CEGUI::MouseCursor::getSingleton().hide();

	MapLoader loader;
	Map* map;
    map = loader.loadMap("lowland.rtm", MyTestApp::getSingleton().getSceneManager(), "Maps");
	map->load();
    // without CEGUI
    //MyTestApp::getSingleton().switchState(new GSMap(map));

	return true;
}

bool MainMenu::buttonConfigPressed(const CEGUI::EventArgs &e)
{	
	ConfigWindow* wnd = new ConfigWindow(mWindow->getParent());
	return true;
}

bool MainMenu::buttonExitPressed(const CEGUI::EventArgs &e)
{
    //MyTestApp::getSingleton().exit();
	return true;
}

bool MainMenu::buttonConsolePressed(const CEGUI::EventArgs &e)
{
    //ConsoleWindow* wnd = new ConsoleWindow(mWindow->getParent());
	return true;
}

}
