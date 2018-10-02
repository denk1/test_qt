#include "ConfigWindow.h"
#include "MyTestApp.h"
#include "MapLoader.h"
#include "GSMainMenu.h"
#include "Config.h"

namespace RAT
{

ConfigWindow::ConfigWindow(CEGUI::Window* parent)
{
    mWindow = CEGUI::WindowManager::getSingleton().loadLayoutFromFile("Config.wnd");
    parent->addChild(mWindow);
	mWindow->setModalState(true);
	try
	{
		CEGUI::Window* buttonContinue = mWindow->getChild("Config/ButtonOK");
		buttonContinue->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&ConfigWindow::buttonOKPressed, this));
	}
	catch(CEGUI::Exception& e)
	{
		// TODO: Add log message
	}
	try
	{
		CEGUI::Window* buttonCancel = mWindow->getChild("Config/ButtonCancel");
		buttonCancel->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&ConfigWindow::buttonCancelPressed, this));
	}
	catch(CEGUI::Exception& e)
	{
		// TODO: Add log message
	}

	try
	{
		CEGUI::Window* camDistScroll = mWindow->getChild("Config/CameraDistanceScroll");
		camDistScroll->subscribeEvent(CEGUI::Scrollbar::EventScrollPositionChanged, CEGUI::Event::Subscriber(&ConfigWindow::distanceScrolled, this));
	}
	catch(CEGUI::Exception& e)
	{
		// TODO: Add log message
	}

	

}

bool ConfigWindow::distanceScrolled(const CEGUI::EventArgs &e)
{
	CEGUI::Scrollbar* scrollbar = dynamic_cast<CEGUI::Scrollbar*>(mWindow->getChild("Config/CameraDistanceScroll"));
	float distance = scrollbar->getScrollPosition() + 10.f;
	Ogre::String value = Ogre::StringConverter::toString(int(distance));
	mWindow->getChild("Config/CameraDistanceCaption")->setText("Camera distance " + value);
	Config::getSingleton().setCameraDistance(distance);
	return true;
}

bool ConfigWindow::buttonOKPressed(const CEGUI::EventArgs &e)
{
	delete this;

	return true;
}

bool ConfigWindow::buttonCancelPressed(const CEGUI::EventArgs &e)
{
	delete this;

	return true;
}

}
