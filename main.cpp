#include "ITS.h"
#include "ConnectionControl.h"
#include "ConnectionControlTcp.h"


using namespace std;

int main()
{
    RAT::ITS its;
    //RAT::ConnectionControl mConnectionControl(&its);
    RAT::ConnectionControlTCP mConnectionControl(&its);
    mConnectionControl.start();
    its.initApp();
    its.getRoot()->startRendering();
    its.closeApp();
    return 0;
}
