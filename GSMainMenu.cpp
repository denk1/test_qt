#include "GSMainMenu.h"
#include "ITS.h"
#include <cegui-0/CEGUI/CEGUI.h>

namespace RAT
{

GSMainMenu::GSMainMenu()
	: GameState(GameState::StMainMenu), mMainMenu(0)
{
	
}

GSMainMenu::~GSMainMenu()
{
	delete mMainMenu;
}

void GSMainMenu::enteredState()
{
	CEGUI::WindowManager &wmgr = CEGUI::WindowManager::getSingleton();
	CEGUI::Window* window = wmgr.createWindow("DefaultWindow", "MainMenu/Background");
	
    CEGUI::System::getSingleton().getDefaultGUIContext().setRootWindow(window);

	mMainMenu = new MainMenu(window);
	mMainMenu->getWindow()->setModalState(true);
	mMainMenu->getWindow()->activate();
}

}
