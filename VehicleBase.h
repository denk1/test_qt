#ifndef VehicleBase_H
#define VehicleBase_H

#include <Ogre.h>

#include "Physics.h"

#include "VehiclePrototype.h"

#define PI 3.14159


namespace RAT
{

	// Базовый класс для автомобиля
	// Инкапсулирует обработку управляющих сигналов (кнопок) и создание
class VehicleBase : public VehiclePrototype, public Ogre::FrameListener
{
public:

		// Сигналы управления автомобилем
	enum VehicleSteer
	{
		VSNone,
		VSLeftPressed,
		VSRightPressed,
		VSForwardPressed,
		VSBackwardPressed,
		VSHandBreakPressed,
		VSLeftReleased,
		VSRightReleased,
		VSForwardReleased,
		VSBackwardReleased,
		VSHandBreakReleased
	};

	enum SpeedType
	{
		Units,
		Kmh,
		Mph
	};

	VehicleBase(const VehiclePrototype& prototype);
	~VehicleBase();

		// Обработка сигналов управления
	void processControl(const VehicleSteer steer);

		// Создание автомобиля
	virtual void create(Ogre::SceneNode* parentSN);

	Ogre::SceneNode* getVehicleSN();


	Ogre::Real getSpeed(const SpeedType speedType = Units);
	Ogre::Real getAvgSpeed(const SpeedType speedType = Units);
    Ogre::Real getCurrSteeringAngle();

	const Ogre::Vector3& getDirection();

	virtual bool frameRenderingQueued(const Ogre::FrameEvent& evt);

    void setAimAngleSteering(const Ogre::Real angle);
    void setCoeffForce(const Ogre::Real coeffForce);

protected:
		// Обновление физики по состоянию управления
	virtual void processIdle();
	virtual void processGas(const bool forward);
	virtual void processBreaks();
	virtual void processHandBreaks();


	void updateVehicle(const Ogre::Real dt);
	void updateVehicleSteering(const Ogre::Real dt);

		// Обработка сигналов управления
	void processNone(){}
	void processLeftPressed();
	void processRightPressed();
	void processForwardPressed();
	void processBackwardPressed();
	void processHandBreakPressed();

	void processLeftReleased();
	void processRightReleased();
	void processForwardReleased();
	void processBackwardReleased();
	void processHandBreakReleased();
    void steeringPlus(const Ogre::Real dt, Ogre::Real speedAffectionIncrement);
    void steeringMinus(const Ogre::Real dt, Ogre::Real speedAffectionIncrement);

	Ogre::SceneNode* mVehicleSN;
	Ogre::SceneNode* mBodySN;
	Ogre::SceneNode* mVehicleWSN[4];

	OgreBulletDynamics::WheeledRigidBody	*mCarChassis;
    OgreBulletDynamics::VehicleTuning		*mTuning;
    OgreBulletDynamics::VehicleRayCaster	*mVehicleRayCaster;
    OgreBulletDynamics::RaycastVehicle		*mVehicle;

	static const int avgSpeedSoft = 20;

	Ogre::Real mSteering;
	Ogre::Real mSpeed;
	Ogre::Real mLastSpeed[avgSpeedSoft];
	int mSpeedPos;
	Ogre::Real mSkipedTime;
	Ogre::Real mAvgSpeed;
	Ogre::Vector3 mPreviosPosition;
	Ogre::Vector3 mDirection;
	
	bool mSteeringLeft;
	bool mSteeringRight;

	bool mGas;
	bool mBreaks;
	bool mHandBreaks;
	
	typedef void (VehicleBase::*VehicleActionPtr) ();
	typedef std::map<VehicleSteer, VehicleActionPtr> TControlMap;
	TControlMap mActionMap;

	typedef std::vector<Ogre::SceneNode*> TSNVec;
	TSNVec mCPNodes;

	const float mSpeedToKmh; 
    const float mSpeedToMph;
    Ogre::Real mAimAngle;
    Ogre::Real mCoeffForce;

    bool flag;
	

};

}

#endif
