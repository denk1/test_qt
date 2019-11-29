#ifndef CONTROLLING_H
#define CONTROLLING_H
#include <thread>
#include <functional>
#include "ITS.h"


namespace RAT {

typedef std::function<void(unsigned char[18])> TypeDefSend;

class ThreadSafeData;

class SenderTelemetry {
public:
    SenderTelemetry(){}

    void send(unsigned char msg[18]) {
        (*_sendFunc)(msg);
    }

    void setSender(TypeDefSend* sendFunc) {
        _sendFunc = sendFunc;
    }

private:
    TypeDefSend* _sendFunc;
};

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
    void setSenderTelemetry(TypeDefSend* sender);
private:
    void run();
    std::thread mThread;
    bool isRun;
    ITS* mPtrITS;
    int mSteeringValue;
    int mMovingValue;
    ThreadSafeData* mPtrThreadSafeData;
    SenderTelemetry mSenderTelemetry;
};
}

#endif
