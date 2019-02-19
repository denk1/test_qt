#include "GSStart.h"
#include "GSMainMenu.h"
#include "ITS.h"

namespace RAT
{

GSStart::GSStart()
	: GameState(GameState::StStart)
{
	// ¬ этом состо€нии возможно будут вступительные ролики. 
	// ѕока сразу переход в главное меню
	mIsActive = true;
    ITS::GetRootS()->addFrameListener(this);
}

void GSStart::startGame()
{
    ITS::getSingleton().switchState(new GSMainMenu());
	
}

GSStart::~GSStart()
{

}

}
