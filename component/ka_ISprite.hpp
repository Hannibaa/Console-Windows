#pragma once
#include <iostream>
#include <console project/basic/ka_IDrawable.hpp>
#include <console project/basic/ka_Transformable.hpp>
#include <console project/console/ka_utility.hpp>
#include <console project/component/ka_ColorString.hpp>



namespace console {

	template<typename Tchar>
	struct StringImage {
		std::basic_string<Tchar>		_sprite;
		int								_lx;
		int								_ly;
	};

	//////////////////////////////////////////////////////////////////////////////////////////////////////////
	// 
	//				BASE CLASS FOR ALL SPRITE DESIGNE   
	// 
	//////////////////////////////////////////////////////////////////////////////////////////////////////////
	
	template<typename Tchar, typename T>
	requires std::is_arithmetic_v<T> 
	class ISprite :public IColorString<Tchar>, public IDrawable, public Transformable<T> {

	public:
		using IColorString<Tchar>::IColorString;
		using IColorString<Tchar>::operator[];
		using IColorString<Tchar>::size;

		fVec2 get_dimension() const override {
			return fVec2{ float(this->size()), 1.f };
		}

		virtual fRect get_bounds() const {
			fRect rect;
			rect.set_position(this->get_position());
			rect.dx = float(this->size());
			rect.dy = 1.f;
			return rect;
		}
	};

//////////////////////////////////////////////////////////////////////////////////////////////////////////
// 
//		 WCHAR_T SPRITE DEFINITION    
// 
//////////////////////////////////////////////////////////////////////////////////////////////////////////


	class WSprite : public ISprite<wchar_t, float> {

		int _lx;
		int _ly;

	public:
		WSprite(std::wstring image, int lx, int ly)
			:_lx(lx), _ly(ly)
		{
			image.resize(_lx * _ly);
			
			set_stringAndColor(image, std::vector<int>(_lx * _ly, COLOR::FG_WHITE));
		}

		WSprite(cwstring cwstr, int lx, int ly)
		{
			set_stringAndColor(cwstr.string(), cwstr.get_attributes());
		}

		WSprite(StringImage<wchar_t> stringImage)
			:WSprite(stringImage._sprite, stringImage._lx, stringImage._ly)
		{}

		fVec2 get_dimension() const override {
			return fVec2{ float(_lx), float(_ly) };
		}

		fRect get_bounds() const override {
			fRect rect;
			rect.set_position(this->get_position());
			rect.dx = _lx;
			rect.dy = _ly;
			return rect;
		}

		void draw() const override {
			auto p = this->get_position();
			for (int i = 0; i < _lx; ++i)
				for (int j = 0; j < _ly ; ++j)
				konsole->draw(p.x + i , p.y + j,
					this->operator[](i + _lx * j).Char, this->operator[](i + _lx * j).Attributes);
		}

	};


//////////////////////////////////////////////////////////////////////////////////////////////////////////
// 
//				USEFULL FUNCTION TO GENERATE IMAGE STRING   
// 
//////////////////////////////////////////////////////////////////////////////////////////////////////////

	void insertx(std::wstring& str,int j, int i0, int i1,  wchar_t w, int lx) {
		for (int i = i0; i < i1; ++i) 	str[i + lx * j] = w;
	}

	void inserty(std::wstring& str,int i,  int j0, int j1, wchar_t w, int lx) {
		for (int j = j0; j < j1; ++j) str[i + lx * j] = w;
	}

	StringImage<wchar_t> make_box(wchar_t wchar, int lx, int ly) {
		std::wstring str(lx * ly, L' ');
		insertx(str, 0		, 0, lx    , wchar, lx);
		inserty(str, 0		, 1, ly    , wchar, lx);
		inserty(str, lx - 1	, 1, ly    , wchar, lx);
		insertx(str, ly - 1	, 1, lx - 1, wchar, lx);

		return StringImage<wchar_t>{ str, lx, ly };
	}

	StringImage<wchar_t> make_box(const wchar_t(&border)[6], int lx, int ly) {
		std::wstring str(lx * ly, L' ');
		str[0] = border[0];
		str[lx - 1] = border[2];
		str[(lx * ly) - 1] = border[4];
		str[lx*(ly - 1)] = border[5];

		insertx(str, 0		, 1, lx - 1, border[1], lx);
		insertx(str, ly - 1	, 1, lx - 1, border[1], lx);
		inserty(str, 0		, 1, ly	- 1	,border[3], lx);
		inserty(str, lx - 1	, 1, ly	- 1	,border[3], lx);

		return StringImage<wchar_t>{str,lx,ly};
	}

}

using wsprite = console::WSprite;