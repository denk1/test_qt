#include "GSStart.h"
#include "GSMainMenu.h"
#include "MyTestApp.h"

namespace RAT
{

GSStart::GSStart()
	: GameState(GameState::StStart)
{
	// ¬ этом состо€нии возможно будут вступительные ролики. 
	// ѕока сразу переход в главное меню
	mIsActive = true;
    MyTestApp::GetRootS()->addFrameListener(this);
}

void GSStart::startGame()
{
    MyTestApp::getSingleton().switchState(new GSMainMenu());
	
}

GSStart::~GSStart()
{

}

}
