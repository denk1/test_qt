#include "Controlling.h"
#include "ThreadSafeData.h"


namespace RAT {



Controlling::Controlling(ITS* ptrITS, ThreadSafeData* ptrThreadSafeData): isRun(false),
                                       mPtrITS(ptrITS),
                                       mSteeringValue(0),
                                       mMovingValue(0),
                                       mPtrThreadSafeData(ptrThreadSafeData)
{

}

Controlling::~Controlling()
{
    isRun = false;
    if(mThread.joinable())
        mThread.join();
}

void Controlling::setSteeringValueFromConn(int v)
{
    mPtrThreadSafeData->setStreeringValue(v);
}

void Controlling::setMovingValueFromConn(int v)
{
    mPtrThreadSafeData->setMovingValue(v);
}

void Controlling::setSteeringValue(int steering)
{
    mSteeringValue = steering;
}

void Controlling::setMovingValue(int moving)
{
    mMovingValue = moving;
}

void Controlling::setVehicleSteeringValue()
{
     mPtrITS->getVehicle()->setAimAngleSteering(mSteeringValue/10.0f);
}

void Controlling::setVehicleMovingValue()
{
    mPtrITS->getVehicle()->setCoeffForce(abs(mMovingValue));
}

void Controlling::setVehicleSteeringNull()
{
    mSteeringValue = 0;
}

void Controlling::setVehicleMovingNull()
{
    mMovingValue = 0;
}

void Controlling::start()
{
    isRun = true;
    mThread = std::thread(&Controlling::run, this);

}


void Controlling::run()
{
    std::this_thread::sleep_for(std::chrono::seconds(10));
    while(isRun) {

        std::this_thread::sleep_for(std::chrono::milliseconds(10));
        if(mSteeringValue) {
            mPtrThreadSafeData->setVehicleSteeringValue();
            if(mSteeringValue > 0)  {
                mPtrITS->getVehicle()->processControl(VehicleBase::VSLeftPressed);
            }
            else {
                mPtrITS->getVehicle()->processControl(VehicleBase::VSRightPressed);
            }



        } else {
            mPtrThreadSafeData->setVehicleSteeringNull();
            //mPtrITS->getVehicle()->setAimAngleSteering(0.0f);
            mPtrITS->getVehicle()->processControl(VehicleBase::VSLeftReleased);
            mPtrITS->getVehicle()->processControl(VehicleBase::VSRightReleased);
        }

        if(mMovingValue) {
            mPtrThreadSafeData->setVehicleMovingValue();
            if(mMovingValue > 0) {
                mPtrITS->getVehicle()->processControl(VehicleBase::VSForwardPressed);
            } else {
                mPtrITS->getVehicle()->processControl(VehicleBase::VSBackwardPressed);
            }


        } else {
            mPtrThreadSafeData->setVehicleMovingNull();
            //mPtrITS->getVehicle()->setCoeffForce(0.0f);
            mPtrITS->getVehicle()->processControl(VehicleBase::VSBackwardReleased);
            mPtrITS->getVehicle()->processControl(VehicleBase::VSForwardReleased);
        }
    }
}


}

