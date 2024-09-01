
#include "include.h"

#include "../src/graphics/ImGuiWindowCrap.h"

#include "../src/back/alocConsole.h"
#include "../src/back/ezAttach.h"

#include "../src/winMainCrap.h"






INT APIENTRY WinMain(HINSTANCE instance, HINSTANCE, PSTR, INT cmd_show)
{
	createConsole();

	ezAttach();

	WinCrap(instance, 0, 0, cmd_show);

	ezDetach();
	
	cin.get();

	return 0;
}
