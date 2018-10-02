#include "IngameMenu.h"
#include "MyTestApp.h"
#include "MapLoader.h"
#include "GSMainMenu.h"

namespace RAT
{

IngameMenu::IngameMenu(CEGUI::Window* parent)
{
    mWindow = CEGUI::WindowManager::getSingleton().loadLayoutFromFile("IngameMenu.wnd");
    parent->addChild(mWindow);

	try
	{
		CEGUI::Window* buttonContinue = mWindow->getChild("IngameMenu/ButtonContinue");
		buttonContinue->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&IngameMenu::buttonContinuePressed, this));
	}
	catch(CEGUI::Exception& e)
	{
		// TODO: Add log message
	}
	try
	{
		CEGUI::Window* buttonReturn = mWindow->getChild("IngameMenu/ButtonReturn");
		buttonReturn->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&IngameMenu::buttonReturnPressed, this));
	}
	catch(CEGUI::Exception& e)
	{
		// TODO: Add log message
	}
	try
	{
		CEGUI::Window* buttonExit = mWindow->getChild("IngameMenu/ButtonExit");
		buttonExit->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&IngameMenu::buttonExitPressed, this));
	}
	catch(CEGUI::Exception& e)
	{
		// TODO: Add log message
	}
		
}


bool IngameMenu::buttonContinuePressed(const CEGUI::EventArgs &e)
{
	mWindow->setModalState(false);
	mWindow->hide();
    CEGUI::System::getSingleton().getDefaultGUIContext().getMouseCursor().hide();

	return true;
}

bool IngameMenu::buttonReturnPressed(const CEGUI::EventArgs &e)
{
	mWindow->setModalState(false);
	mWindow->hide();

    MyTestApp::getSingleton().switchState(new GSMainMenu());

	return true;
}

bool IngameMenu::buttonExitPressed(const CEGUI::EventArgs &e)
{
	mWindow->setModalState(false);
	mWindow->hide();

    //MyTestApp::getSingleton().exit();

	return true;
}

}
