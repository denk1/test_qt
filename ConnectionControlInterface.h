#ifndef CONNECTION_CONTROL_INTERFACE_H
#define CONNECTION_CONTROL_INTERFACE_H
#include "ITS.h"
#include <thread>
#include <chrono>

namespace RAT {


class ConnectionControlInterface {
public:
    ConnectionControlInterface(ITS* ptrITS);
    virtual void start();
protected:
    ITS* mPtrITS = nullptr;
    std::thread mThread;
    virtual void run()=0;
    virtual void set_timer(int i) = 0;
};

}
#endif
