#include "ITS.h"

using namespace std;

int main()
{
    RAT::ITS its;
    its.initApp();
    its.getRoot()->startRendering();
    its.closeApp();
    return 0;
}
