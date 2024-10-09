#pragma once
#include <Windows.h>
#include <iostream>

#define end_  '\n'


namespace win {

	void show_popupmenu(HANDLE hstdout, COORD mouse)
	{
		// Create popup menu 
		HMENU menu = CreatePopupMenu();

		// add menu items
		AppendMenu(menu, MF_STRING, 1, TEXT("Blue"));
		AppendMenu(menu, MF_STRING, 2, TEXT("Close"));

		// display menu
		DWORD cmd = TrackPopupMenu(menu, 
									TPM_RETURNCMD | TPM_NONOTIFY,
									mouse.X, mouse.Y,
									0, 
									GetConsoleWindow(), NULL);

		// Handle menu selection
		switch (cmd) {
		case 1:
			std::cout << "option 1" << end_;
			break;
		case 2:
			std::cout << "option 2" << end_;
			break;
		default:
			std::cout << "No selection" << end_;
			break;

		}

		DestroyMenu(menu);
	}


}



