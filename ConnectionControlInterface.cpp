#include "ConnectionControlInterface.h"

namespace  RAT {

ConnectionControlInterface::ConnectionControlInterface(ITS *ptrITS) :mPtrITS(ptrITS)
{

}

void ConnectionControlInterface::start()
{
    mThread = std::thread(&ConnectionControlInterface::run, this );
}

}
