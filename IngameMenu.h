#ifndef IngameMenu_H
#define IngameMenu_H

#include <cegui-0/CEGUI/CEGUI.h>
#include "Window.h"
#include "Map.h"

namespace RAT
{

class IngameMenu : public Window
{
public:
	IngameMenu(CEGUI::Window* parent);
	
	// ~Window() Используем стандартный деструктор Window

protected:
	// События нажатия на кнопки
	bool buttonContinuePressed(const CEGUI::EventArgs &e);
	bool buttonReturnPressed(const CEGUI::EventArgs &e);
	bool buttonExitPressed(const CEGUI::EventArgs &e);

	

};

}

#endif
