#ifndef ConfigWindow_H
#define ConfigWindow_H

#include <cegui-0/CEGUI/CEGUI.h>
#include "Window.h"
#include "Map.h"

namespace RAT
{

class ConfigWindow : public Window
{
public:
	ConfigWindow(CEGUI::Window* parent);

protected:

	bool buttonOKPressed(const CEGUI::EventArgs &e);
	bool buttonCancelPressed(const CEGUI::EventArgs &e);

	bool distanceScrolled(const CEGUI::EventArgs &e);
	

};

}

#endif
