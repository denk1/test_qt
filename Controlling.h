#ifndef CONTROLLING_H
#define CONTROLLING_H
#include <thread>
#include "ITS.h"


namespace RAT {
class ThreadSafeData;
class Controlling {
public:
    Controlling(ITS* ptrITS, ThreadSafeData* ptrThreadSafeData);
    ~Controlling();
    void setSteeringValueFromConn(int v);
    void setMovingValueFromConn(int v);
    void setSteeringValue(int steering);
    void setMovingValue(int moving);
    void setVehicleSteeringValue();
    void setVehicleMovingValue();
    void setVehicleSteeringNull();
    void setVehicleMovingNull();
    void start();
private:
    void run();
    std::thread mThread;
    bool isRun;
    ITS* mPtrITS;
    int mSteeringValue;
    int mMovingValue;
    ThreadSafeData* mPtrThreadSafeData;
};
}

#endif
