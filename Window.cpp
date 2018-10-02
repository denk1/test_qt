#include "Window.h"

namespace RAT
{

Window::Window()
	:mWindow(0)
{

}

Window::Window(CEGUI::Window* window)
	:mWindow(window)
{

}


CEGUI::Window* Window::getWindow()
{
	return mWindow;
}

Window::~Window()
{
    mWindow->getParent()->removeChild(mWindow);
	CEGUI::WindowManager &wmgr = CEGUI::WindowManager::getSingleton();
	wmgr.destroyWindow(mWindow);
}

}
