#include "common.h"
#include "lp_processerror.h"



void RedefinedConsole(void)
{
#ifdef _WIN32
	HMENU hMenu = GetSystemMenu(GetConsoleWindow(), FALSE);
	EnableMenuItem(hMenu, SC_CLOSE, MF_GRAYED);
#endif
}
