#ifndef GSMainMenu_H
#define GSMainMenu_H

#include "GameState.h"
#include "MainMenu.h"

namespace RAT
{

	// Состояние игры - Главное меню.
class GSMainMenu : public GameState
{
public:
	GSMainMenu();

protected:

	~GSMainMenu();

	virtual void enteredState();

	MainMenu* mMainMenu;



};

}

#endif