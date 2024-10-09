/* UTILISE ONLY MACRO CODE */

#pragma once
#include <include/Macros.hpp>
#include <include/console/Escape_macros.h>
#include <console project/console/ka_color.hpp>



//#define   OUT(Type, Cchar)  			if constexpr (std::is_same_v<Type, char>)		/		
//	std::cout << TEXT_COLOR(Cchar.Attributes,  Cchar.Attributes, 	Cchar.Char);		/	
//else if constexpr (std::is_same_v<Type, wchar_t>)										/		
//									std::wcout << Cchar;
//



namespace console {

	template<typename Tchar>
	class IPrintable {
	public:
		virtual void print() const = 0;

		virtual ~IPrintable() = default;
	};


}