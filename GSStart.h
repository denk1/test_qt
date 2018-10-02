#ifndef GSStart_H
#define GSStart_H

#include "GameState.h"

namespace RAT
{

	// Состояние игры - Инициализация
class GSStart : public GameState
{
public:
	GSStart();

		// Инициализирует это состояние, как начальное состояние игры
	void startGame();

protected:

	~GSStart();

		// Не вызывается, т.к. это первое состояние
	virtual void enteredState(){}



};

}

#endif
