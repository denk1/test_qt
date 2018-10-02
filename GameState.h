#ifndef GameState_H
#define GameState_H

#include <Ogre.h>

namespace RAT
{

	// јбстрактный класс, определ€ющий общее состо€ние игры
class GameState :  public Ogre::FrameListener
{
public:
		// ѕеречисление возможных состо€ний, реализивонных соответствующими классами.
	enum State
	{
		StStart,
		StMainMenu,
		StLoading,
		StMap,
		StServerList,
		StEnd
	};
		// ¬ конструкторе потомков должна быть только базова€ инициализаци€ состо€ни€, остальное в enteredState()
	GameState(const State state);

	State getState() const;

		// «апрос на смену состо€ни€. ѕеред сменой текущее состо€ние будет удалено. 
		// ”даление состо€ни€ возможно только этим методом, причина: FrameListner.
	virtual void switchState(GameState* nextState);

protected:

	virtual ~GameState();

		// Ётот метод вызываетс€ после того как предыдущее состо€ние завершилось, 
		// игра перешла в новое состо€ние и осуществлена подписка Ogre::FrameListener
	virtual void enteredState() = 0;

		// Ogre::FrameListener
	virtual bool frameEnded(const Ogre::FrameEvent& evt);

	State mState;
	GameState* mNextState;
	bool		mIsActive; // явл€етс€ ли состо€ние активным

private:

		// ѕерейти в новое состо€ние. Ќе переопредел€етс€.
	void applyState();

};

}

#endif
