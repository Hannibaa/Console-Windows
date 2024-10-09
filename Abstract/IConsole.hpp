/* 
				INTERFACE FOR TEXT CONSOLE OBJECT
				AUTHOR : KADDA AOUES
				DATE   : 9 / 9 / 2024
				NAMESPACE : ABSTRACT OR ABS 
*/

#pragma once



namespace abstract {

	class IConsole {

	public:
		virtual int construct(int, int, int, int) = 0;
		virtual int is_open() const = 0;
		virtual int display() = 0;
		virtual bool pull_event() = 0;
		virtual bool wait_event() = 0;


		virtual ~IConsole() = default;
	};


}


