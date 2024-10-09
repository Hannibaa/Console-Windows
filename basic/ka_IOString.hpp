#pragma once
#include <Windows.h>
#include <iostream>
#include <thread>
#include <vector>
#include <string>
//#include <include/string/string_utility.h>




namespace console {


	template<typename Tchar>
	class IOString {

		//IO_device&							_device;
		std::vector<Tchar>					_buffer;
		HANDLE								_stdin;
		DWORD								_dwOldMode;
		bool								_valide;
		bool								_read;
	public:

		IOString(/*IO_device& device, */HANDLE hstdin = INVALID_HANDLE_VALUE)
			:// _device(device)
			 _buffer{}
			, _stdin{hstdin}
			, _dwOldMode{}
			, _valide{false}
			, _read{true}
		{
			if (_stdin == INVALID_HANDLE_VALUE)
			{
				_stdin = GetStdHandle(STD_INPUT_HANDLE);
				if (_stdin == INVALID_HANDLE_VALUE) {
					std::cerr << "Error in getting input";
					_valide = false;
				}
				else
					_valide = true;
			}
			else
			{
				_valide = true;
			}
			
			if (_valide)
			{
				_valide = true;
				GetConsoleMode(_stdin, &_dwOldMode);
				SetConsoleMode(_stdin, _dwOldMode &
					~(ENABLE_LINE_INPUT | ENABLE_ECHO_INPUT));
			}

		}

		void clear() {
			_buffer.clear();
		}

		bool reading() const {
			return _read;
		}

		void read() {
			_read = true;
		}

		virtual ~IOString() {
			SetConsoleMode(_stdin, _dwOldMode);
		}

		void String(std::basic_string<Tchar>& string) {
			INPUT_RECORD ir{};
			DWORD  event{};

			// enter text : 
			if (_valide && _read) {

				// Read input event
				ReadConsoleInput(_stdin, &ir, 1, &event);

				if (ir.EventType == KEY_EVENT)
				{
					KEY_EVENT_RECORD  ekey = ir.Event.KeyEvent;

					// check if key is pressed
					if (ekey.bKeyDown)
					{
						// Handle special key
						if (ekey.wVirtualKeyCode == VK_RETURN) {
							_read = false;
							return; // finish reading
						}
						else if (ekey.wVirtualKeyCode == VK_BACK && ! _buffer.empty())
						{
							_buffer.pop_back();
						}
						else if (ekey.uChar.AsciiChar > 31 && ekey.uChar.AsciiChar < 127)
						{
							// using vector of char 
							_buffer.push_back(static_cast<Tchar>(ekey.uChar.AsciiChar));							
						}
					}
				}

				string = std::basic_string<Tchar>(_buffer.begin(), _buffer.end());

			}// valid and read
		}// End String()


	};
	


}