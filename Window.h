#ifndef Window_H
#define Window_H

#include <cegui-0/CEGUI/CEGUI.h>

namespace RAT
{

class Window
{
public:

	Window();
	Window(CEGUI::Window* window);
	virtual ~Window();

	CEGUI::Window* getWindow();

protected:
	CEGUI::Window* mWindow;

};

}

#endif
