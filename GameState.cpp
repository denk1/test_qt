#include "GameState.h"
#include "ITS.h"

namespace RAT
{

GameState::GameState(const State state)
		: mState(state), mNextState(0), mIsActive(false)
{
	
}

GameState::~GameState()
{
    ITS::GetRootS()->removeFrameListener(this);
}

GameState::State GameState::getState() const
{
	return mState;
}

void GameState::switchState(GameState* nextState)
{
	mNextState = nextState;
}

void GameState::applyState()
{
		// Подписываемся
    ITS::GetRootS()->addFrameListener(this);

		// callback об успешном входе в состояние
	enteredState();

	mIsActive = true;
}

bool  GameState::frameEnded(const Ogre::FrameEvent& evt)
{
		// Переключение состояния. Удалять FrameListner можно только в конце кадра, после вызова frameEnded
	if (mNextState)
	{
		GameState* nextState = mNextState;
		delete this;

			// Входим в новое состояние
		nextState->applyState();
		
	}

	return true;
}

}	
