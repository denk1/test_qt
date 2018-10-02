#ifndef MainMenu_H
#define MainMenu_H

#include <cegui-0/CEGUI/CEGUI.h>
#include "Window.h"
#include "Map.h"

namespace RAT
{

class MainMenu : public Window
{
public:
	MainMenu(CEGUI::Window* parent);

	// ~Window() Используем стандартный деструктор Window
protected:
	// События нажатия на кнопки
	bool buttonStartPressed(const CEGUI::EventArgs &e);
	bool buttonConfigPressed(const CEGUI::EventArgs &e);
	bool buttonExitPressed(const CEGUI::EventArgs &e);
	bool buttonConsolePressed(const CEGUI::EventArgs &e);
};

}

#endif
