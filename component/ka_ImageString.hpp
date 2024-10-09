#pragma once
#include <iostream>
#include <console project/console/ka_utility.hpp>
#include <console project/component/ka_IColorString.hpp>
#include <console project/basic/ka_Transformable.hpp>
#include <console project/basic/ka_IPrintable.hpp>



namespace console {

	template<typename Tchar>
	class ImageString : public IColorString<Tchar>, public Transformable<float>, public IDrawable
	{
		int						_lx;
		int						_ly;

	public:
		using IColorString<Tchar>::IColorString;
		using IColorString<Tchar>::operator[];
		using IColorString<Tchar>::size;


		void set_size(int lx, int ly) {
			_lx = lx;
			_ly = ly;
		}

		virtual void draw() const override {
			auto p = this->get_position();
			for (int i = 0; i < _lx; ++i)
				for (int j = 0; j < _ly; ++j)
				konsole->draw(p.x + i, p.y + j, 
					this->operator[](j*_lx + i).Char, this->operator[](j*_lx + i).Attributes);
		}

		fVec2 get_dimension() const override {
			return fVec2{ float(_lx), float(_ly)};
		}

		fRect get_bounds() const {
			fVec2 p = this->get_position();
			return fRect(p.x, p.y, _lx, _ly);
		}
	};


	// stream operator << 
	template<typename TChar>
	std::basic_ostream<TChar>& operator << (std::basic_ostream<TChar>& os,
		const ImageString<TChar>& str)
	{
		os << str.string();
		return os;
	}

}

using wstringImg = console::ImageString<wchar_t>;
using stringImg = console::ImageString<wchar_t>;
