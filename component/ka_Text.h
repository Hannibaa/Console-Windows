/* STRING THAT USING ESCAPE CODE FOR DRAWING */
#pragma once
#include <console project/component/ka_IColorString.hpp>
#include <console project/basic/ka_IPrintable.hpp>
#include <include/console/Escape_macros.h>


namespace console {
	template<typename Tchar>
	class Text : public IColorString<Tchar>, public IPrintable<Tchar>
	{
	public:
		using IColorString<Tchar>::IColorString;
		using IColorString<Tchar>::operator[];
		using IColorString<Tchar>::size;


		virtual void print() const override {
			auto p = this->get_position();

#define THIS(i)     this->operator[](i)
#define BG(c)		KA_OCT(c)

			for (int i = 0; i < this->size(); ++i)
			{
				if constexpr (std::is_same_v<Tchar, char>) {
					std::cout << TEXT_COLOR(KA_LOCT(
						KA_LOCT(THIS(i).Attributes)),
						KA_HOCT(THIS(i).Attributes),
						THIS(i).Char);
				}
				else if constexpr (std::is_same_v<Tchar, wchar_t>) {
					std::wcout << WTEXT_COLOR(THIS(i).Attributes, THIS(i).Attributes, THIS(i).Char);
				}
				// TODO manage other case : 
			}
		}

		~Text() = default;
	private:
	};

}