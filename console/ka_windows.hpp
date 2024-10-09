/*
 *****************************************************************************
 *                                                                           *
 *                          Platform Independent                             *
 *                   Windows Tools For Console Library                       *
 *                                                                           *
 * Author: KADDA Aoues - haoues_k@yahoo.fr - 2023                            *
 *                                                                           *
 * URL :                                                                     *
 *                                                                           *
 * Copyright notice: Not yet                                                 *
 * Free use of the Library CFile                                             *
 * is permitted under the guidelines and in accordance with the most current *
 * version of the ??MIT License??.                                           *
 * http://www.opensource.org/licenses/MIT                                    *
 *                                                                           *
 *****************************************************************************
*/

/*
				This from "C++ for game programmers"

	First, I would like to thank Professor Sandy Hill from University of Massachusetts
	Amherst, for doing such a fantastic job teaching computer graphics and motivating
	me to go further and learn new things, It was thanks to his classes and guidance
	that I eventually chose the career path I did, and ended up in the game industry.

	Many thanks t Ned Way, Not only did he review parts of the manuscript and help out
	with some fo the low-level details, but he also put up with me at work for four
	years, during which time I learned a lot from him.

	Also, thanks to Tom Whittaker, Kyle Wilson, and Adrian Stone for their helpful comments
	and interesting discussions on different C++ topics that helped shape this book.

	And thanks to all the game developers out there who are actively sharing information
	by writing books and magazine articles, giving talks, or even by participati8ng in
	the game-development mailing lists. In particular, thanks to all the folks on the
	sweng-gamedev mailing list for a lot of interesting discussions, even if we do not
	always agree on everything. Without that spirit of sharing, we would all be suck
	learning the same things over and over by ourselves, and not make much progress.

	Most importantly, a big thanks to Holly Ordway, my wife. There is no way this
	book would have been possible without her encouragement and support, both moral
	and practical, since she tirelessly proofread every single chapter and offered
	many useful suggestions. Mipmap, our cat, helped in his own way by purring on my
	lap and not letting me get up, so I was forced to finish the book faster.
*/

/**********************************************************************************
 *
 * Copyright (c) 2022-2023 KADDA Aoues
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *
 **********************************************************************************/



#pragma once
#include <Windows.h>
#include <iostream>
#include <utility>





#define KeyPressed(Key)            GetAsyncKeyState(Key) & 0x8000
#define KeyReleased(Key)           GetAsyncKeyState(Key) & 0x0001
#define KeyOn(Key)                 GetAsyncKeyState(Key) & 0x8001

#define TITLE(wtitle)              SetConsoleTitle(wtitle)

#define _u(Char)                   ((unsigned)Char)
#define Get_Escape                 get_out

bool get_out() {

	if (KeyOn(VK_ESCAPE)) return true;
	else return false;

}


namespace win {

	LONG  g_oldStyle{};

	//////////////////////////////////////////////////////////////////////////////////////////////////////////
	// 
	// this function will be called only one time
	// return true if get called only one time; 
	// other wise will get false because already called before.
	// 
	//////////////////////////////////////////////////////////////////////////////////////////////////////////
	// 
	//    Functions for manipulate a console windows style and event outof console windows
	// 
	//////////////////////////////////////////////////////////////////////////////////////////////////////////

	bool Get_ConsoleWindow_Style() {
		HWND  hwndConsole = GetConsoleWindow();
		static bool oneTime = true;

		if (oneTime) {
			g_oldStyle = GetWindowLong(hwndConsole, GWL_STYLE);
			oneTime = false;
			return true;
		}

		return false;
	}


	bool Set_Window_Style(HWND hwnd, LONG style) {
		// set the new style
		SetWindowLong(hwnd, GWL_STYLE, style);
		if (SetWindowPos(hwnd, NULL, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE | SWP_FRAMECHANGED))
			return true;
		else
			return false;
	}

	bool Set_ConsoleWindow_Style() {
		HWND  hwndConsole = GetConsoleWindow();

		auto style = GetWindowLong(hwndConsole, GWL_STYLE);

		// modify style
		style &= ~WS_SIZEBOX; // unable resizing.
		style &= ~WS_SYSMENU; // remove system menu
		style &= ~WS_HSCROLL; // remove scrolling horizotall
		style &= ~WS_VSCROLL; // remove vertical scrolling
		//...

		// set the new style
		return Set_Window_Style(hwndConsole, style);
	}

	bool Set_ConsoleWindow_OldStyle() {
		HWND  hwndConsole = GetConsoleWindow();
		return Set_Window_Style(hwndConsole, g_oldStyle);
	}

	// Get Windows Position and Size
	std::pair<COORD, COORD> get_console_rect() {
		HWND hwndConsole = GetConsoleWindow();

		RECT rect{};

		if (GetWindowRect(hwndConsole, &rect)) {
			return std::make_pair(COORD(rect.left, rect.top), COORD(rect.right, rect.bottom));
		}
		else
		{
			std::cerr << "Error getting window rectangle\n";
			return {};
		}

	}

	bool set_console_rect(int x, int y, int lx, int ly) {
		HWND hwndConsole = GetConsoleWindow();

		const UINT swp_flags = SWP_NOZORDER | SWP_NOACTIVATE | SWP_SHOWWINDOW;

		if (SetWindowPos(hwndConsole, NULL, x, y, x + lx, y + ly, swp_flags)) {

			return true;
		}
		else {
			std::cerr << "Error in Set Window Position \n";
			return false;
		}
	}

	//////////////////////////////////////////////////////////////////////////////////////////////////////////
	// 
	//     Get Coordinate of Size and maximum Size of console window
	// 
	//////////////////////////////////////////////////////////////////////////////////////////////////////////

	std::pair<COORD, COORD>  get_console_size() {
		CONSOLE_SCREEN_BUFFER_INFO csbi{};

		if (GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi))
		{
			return std::make_pair(csbi.dwSize, csbi.dwMaximumWindowSize);
		}
		else {
			return {};
		}
	}

	// Set console to full screen mode.
	bool fullscreen_console() {
		bool success = ShowWindow(GetConsoleWindow(), SW_MAXIMIZE);
		SendMessage(GetConsoleWindow(), WM_SYSKEYDOWN, VK_RETURN, 0x20000000);
		return success;
	}


	std::pair<COORD, COORD>  get_console_window_rect() {
		HWND hwind = GetConsoleWindow();

		RECT rect{};
		GetWindowRect(hwind, &rect);

		COORD coord = { rect.left, rect.top };
		COORD sizexy = { rect.right - rect.left, rect.bottom - rect.top };

		return std::pair(coord, sizexy);
	}


	void move_console_window(int x, int y) {
		HWND hwind = GetConsoleWindow();

		RECT rect{};
		GetWindowRect(hwind, &rect);

		MoveWindow(hwind, x, y, rect.right - rect.left, rect.bottom - rect.top, true);
	}

	//////////////////////////////////////////////////////////////////////////////////////////////////////////
	// 
	//    GET GLOBALE MOUSE COORDINATE
	// 
	//////////////////////////////////////////////////////////////////////////////////////////////////////////

	POINT get_mouseCoord() {
		POINT p{};
		GetCursorPos(&p);
		return p;
	}
}