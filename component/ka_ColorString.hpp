#pragma once
#include <iostream>
#include <console project/console/ka_utility.hpp>
#include <console project/component/ka_IColorString.hpp>
#include <console project/basic/ka_Transformable.hpp>
#include <console project/basic/ka_IPrintable.hpp>



namespace console {

	template<typename Tchar>
	class ColorString : public IColorString<Tchar>, public Transformable<float>, public IDrawable
	{
	public :
		using IColorString<Tchar>::IColorString;
		using IColorString<Tchar>::operator[];
		using IColorString<Tchar>::size;

		virtual void draw() const override{
			auto p = this->get_position();
			for (int i = 0 ; i < this->size() ;++i)
			konsole->draw(p.x + i, p.y, this->operator[](i).Char, this->operator[](i).Attributes);
		}

		constexpr fVec2 get_dimension() const override {
			return fVec2{ float(this->size()), 1.f };
		}

		constexpr fRect get_bounds() const {
			fVec2 p = this->get_position();
			fVec2 l = this->get_dimension();
			return fRect(p.x, p.y, l.x, l.y);
		}
	};


	// stream operator << 
	template<typename TChar>
	std::basic_ostream<TChar>& operator << (std::basic_ostream<TChar>& os,
		const ColorString<TChar>& str)
	{
		os << str.string();
		return os;
	}

}

using cwstring = console::ColorString<wchar_t>;
using cstring = console::ColorString<char>;
