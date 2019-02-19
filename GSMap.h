#ifndef GSMap_H
#define GSMap_H

#include "GameState.h"
#include "Map.h"
#include "Vehicle.h"
#include "ITS.h"

namespace RAT
{

class ITS;

	// Состояние игры - Карта(Уровень)
class GSMap : public GameState
{
public:
    GSMap(Map* map, ITS* app);
    VehicleBase *getVehicle();

protected:

	~GSMap();

	virtual void enteredState();


	Map* mMap;


	// Ogre::FrameListner
	virtual bool frameStarted(const Ogre::FrameEvent& evt);
	virtual bool frameRenderingQueued(const Ogre::FrameEvent& evt);
	virtual bool frameEnded(const Ogre::FrameEvent& evt);

	Vehicle* mVehicle;

	
	// tmp
	bool mInVehicle;
private:
    ITS* mIts;


};

}

#endif
