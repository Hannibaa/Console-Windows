#pragma once
// Statu bar design



#include <console project/console/ka_utility.hpp>
#include <console project/component/ka_Draw.hpp>





namespace TextUI {

	// this function colored or attributed first character drawed in menu.
	static void draw_string_menu(int x, int y, 
		const							std::wstring wstr, 
		int								colorfg, 
		int								colorbg,
		int								color_firstchar
	) 
	{

		konsole->draw(x, y, wstr[0], color_firstchar | colorbg);
		for (int i = 1; i < wstr.size(); ++i)
			konsole->draw(x + i, y, wstr[i], colorfg | colorbg);
	}


	void StatuBar(int x, int y,						// coordinate where we put bar sub menu
		int step,									// step between items
		const std::vector<std::wstring>& vItems, 	// items in form of string 
		int colorfg, 								// general foreground color
		int colorbg,								// general background color
		int firstCharColor,							// first char color foreground
		int first_step = 1							// first step space
	)
	{
		// using background black :
		int width = konsole->width();
		int hight = konsole->hight();
		int l{ first_step };

		Draw::box(x, y, width, 1, colorfg, colorbg);

		for (const auto& str : vItems)
		{
			draw_string_menu(x + l, y, str, colorfg, COLOR::BG_BLACK, firstCharColor);
			l += str.size() + step;
			if (l > width) break;
		}
	}

	
	using strVectorBuilder = console::VectorBuilder<std::wstring>;

	void StatuBar(const std::vector<std::wstring>& items, int index = 1) {
		auto Hight = konsole->hight();
		StatuBar(0, Hight - index, 3, items, COLOR::FG_YELLOW, COLOR::FG_DARK_YELLOW << 4, COLOR::FG_RED);

	}


//////////////////////////////////////////////////////////////////////////////////////////////////////////
// 
//    STATUS BAR STRUCTURE
// 
//////////////////////////////////////////////////////////////////////////////////////////////////////////

	struct  sStatuBar {
		int x;
		int y;

		int step;
		int first_step;

		std::vector<std::wstring>	vItems;

		int color_fg;
		int color_bg;
		int color_firstChar;
	};


	class StatuBarBuilder {
		sStatuBar _statuBar;

	public:
		StatuBarBuilder& set_position(int x, int y) {
			_statuBar.x = x;
			_statuBar.y = y;

			return *this;
		}

		StatuBarBuilder& set_step(int _step, int _firstStep) {
			_statuBar.step = _step;
			_statuBar.first_step = _firstStep;

			return *this;
		}

		StatuBarBuilder& set_colorFG(int col) {
			_statuBar.color_fg = col;
			return *this;
		}

		StatuBarBuilder& set_colorBG(int col) {
			_statuBar.color_bg = col;
			return *this;
		}

		StatuBarBuilder& add_item(const std::wstring& item) {
			_statuBar.vItems.push_back(item);

			return *this;
		}

		StatuBarBuilder& add_vecItems(const std::vector<std::wstring>& vecStr) {
			for(const auto& s : vecStr) _statuBar.vItems.push_back(s);

			return *this;
		}

		StatuBarBuilder& set_colorChar(int col) {
			_statuBar.color_firstChar = col;
			return *this;
		}

		sStatuBar get() const {
			return std::move(_statuBar);
		}
	};


	void StatuBar(const sStatuBar& statuBar) {
		StatuBar(statuBar.x, statuBar.y, statuBar.step, statuBar.vItems,
			statuBar.color_fg, statuBar.color_bg, statuBar.color_firstChar, statuBar.first_step);
	}

}