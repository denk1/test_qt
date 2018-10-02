#ifndef GSMap_H
#define GSMap_H

#include "GameState.h"
#include "Map.h"
#include "IngameMenu.h"
#include "Vehicle.h"

namespace RAT
{

	// Состояние игры - Карта(Уровень)
class GSMap : public GameState
{
public:
	GSMap(Map* map);

protected:

	~GSMap();

	virtual void enteredState();

		// Ввод пользователя
	bool keyDown(const CEGUI::EventArgs &e);
	bool keyUp(const CEGUI::EventArgs &e);

	bool mouseDown(const CEGUI::EventArgs &e);
	bool mouseUp(const CEGUI::EventArgs &e);
	bool mouseMove(const CEGUI::EventArgs &e);

	Map* mMap;

	IngameMenu* mIngameMenu;

	// Ogre::FrameListner
	virtual bool frameStarted(const Ogre::FrameEvent& evt);
	virtual bool frameRenderingQueued(const Ogre::FrameEvent& evt);
	virtual bool frameEnded(const Ogre::FrameEvent& evt);

	Vehicle* mVehicle;

	typedef std::vector<CEGUI::Event::Connection> StateEventList;
	StateEventList mEvents;
	
	// tmp
	bool mInVehicle;


};

}

#endif