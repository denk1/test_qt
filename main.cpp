#include "MyTestApp.h"

using namespace std;

int main()
{
    RAT::MyTestApp app;
    app.initApp();
    app.getRoot()->startRendering();
    app.closeApp();
    return 0;
}
