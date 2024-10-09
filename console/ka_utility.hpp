/*										Utility Console								*/
#pragma once


#include <iostream>
#include <vector>
#include <sstream>
#include <cwctype>
#include <tuple>
#include <array>
#include <utility>
#include <include/time/Game_timer.h>
#include <console project/console/ka_console.hpp>
#include <include/geometry/vector2d.hpp>
#include <include/geometry/rectangle.hpp>
#include <include/metaprogramming/my_metaprog.h>



#define MAX_CHAR_TITLE				  256
#define PRINT_TITLE(format, ... )     swprintf_s(console::title, MAX_CHAR_TITLE - 1, format , __VA_ARGS__); \
                                      TITLE(console::title)


//////////////////////////////////////////////////////////////////////////////////////////////////////////
// 
//    make unique pointer to console as globale variable
//	  Konsole as unique pointer to object Console
// 
//////////////////////////////////////////////////////////////////////////////////////////////////////////

using Konsole = std::unique_ptr<console::Console>;

Konsole  konsole = std::make_unique<console::Console>();


namespace console {

	// 
	// We will make also globle variable Need it for game programming
	// 
	float				 fps{ 1.f };					// frame per seconds = 1.f / elaps
	float				 elaps{};						// elapsed time
	float				 Dt60{ 1.5f };					// dalta time in 60 seconds - steps time -
	float				 Dt{};						    // Dt = Dt60 * 60.f / fps;
	wchar_t				 title[MAX_CHAR_TITLE]{};       // buffer for title 

#define		 ka_fps      console::fps
#define		 ka_elaps	 console::elaps
#define		 ka_Dt60	 console::Dt60
#define		 ka_Dt  	 console::Dt

	// Define Macro to calculate fps and elaps timer without repeating code every time
#define KA_FPS(timer)     				ka_elaps = timer##.get_elapsed_time();    \
										timer##.reset();						  \
										ka_fps = 1.f / ka_elaps;

#define KA_MAKE_FPS(timer)				KA_FPS(timer)

#define KA_MAKE_DALTA_TIME(Float)		ka_Dt60		  = float(Float)	;		  \
										ka_Dt		  = ka_Dt60 * 60.f * ka_elaps; 

	const wchar_t Lborder[6] = { 0x250c ,0x2500,0x2510, 0x2502 ,0x2518,0x2514 }; // light border;
	const wchar_t Hborder[6] = { 0x250f ,0x2501,0x2513, 0x2503 ,0x251b,0x2517 }; // heavy border;

	const wchar_t Hvborder[6] = {};
	const wchar_t Hhborder[6] = {};

	const wchar_t b_ = L'[';
	const wchar_t _e = L']';

//////////////////////////////////////////////////////////////////////////////////////////////////////////
// 
//    MENU AND SOME FUNCTION
// 
//////////////////////////////////////////////////////////////////////////////////////////////////////////
	


	// Drawing string at coordinate x,y with color fg and bg :
	void draw(int x, int y, const std::wstring wstr, int colorfg, int colorbg) {
		int i{};
		for (const auto& c : wstr)
			konsole->draw(x + (++i), y, c, colorfg | colorbg << 4);
	}

	void draw_h(int x, int y, const std::wstring wstr, int colorfg, int colorbg) {
		int i{};
		for (const auto& c : wstr)
			konsole->draw(x , y + (++i), c, colorfg | colorbg << 4);
	}

	// Drawing methode defined here
	void box(int x, int y, int lx, int ly, int colorfg, int colorbg = -1)
	{
		if (colorbg == -1) colorbg = konsole->get_background_color();

		for (int i = x; i < x + lx; ++i)
			for (int j = y; j < y + ly; ++j)
			{
				konsole->draw(i, j, L' ', colorfg | colorbg);
			}

	}

	void box(const iRect& rect, int _colorfg, int _colorbg = -1) {
		box(rect.x, rect.y, rect.dx, rect.dy, _colorfg, _colorbg);
	}

	template<typename TRect, typename T = typename TRect::value_type>
		requires std::is_arithmetic_v<T> || std::is_enum_v<T> &&
	std::is_arithmetic_v<typename TRect::value_type>
		void box(const TRect & rect, int _colorfg, int _colorbg = -1) {
		box(
			static_cast<int>(rect.x),
			static_cast<int>(rect.y),
			static_cast<int>(rect.dx),
			static_cast<int>(rect.dy), _colorfg, _colorbg);
	}

	void shadow(const iRect& box, int color_shader)
	{
		for (int i = box.x + 1; i < box.x + 1 + box.dx; ++i)
			konsole->draw(i, box.y + box.dy, L' ', color_shader);

		for (int j = box.y + 1; j < box.y + 1 + box.dy; ++j)
			konsole->draw(box.x + box.dx, j, L' ', color_shader);
	}

	//////////////////////////////////////////////////////////////////////////////////////////////////////////
	// 
	//		MESSAGE BOX THAT HAVE BUTTON    
	// 
	//////////////////////////////////////////////////////////////////////////////////////////////////////////

	void messageBox(const iRect& rbox, const std::wstring& wmsg, int colorfg = COLOR::FG_WHITE,
		int colorbg = COLOR::BG_BLUE,
		int colorshadow = COLOR::BG_DARK_GREY)
	{
		box(rbox, colorfg, colorbg);

		if (colorshadow != -1)
			shadow(rbox, colorshadow);

		konsole->text_at(rbox.x + 1, rbox.y + 1, wmsg, colorfg | colorbg);
	}

	void messageBox(int x, int y, const std::wstring& wmsg, int colorfg = COLOR::FG_WHITE,
		int colorbg = COLOR::BG_BLUE,
		int colorshadow = COLOR::BG_DARK_GREY)
	{
		iRect  r;
		r.x = x - 2;
		r.y = y - 1;
		r.dx = 4 + static_cast<int>(wmsg.size());
		r.dy = 3;

		messageBox(r, wmsg, colorfg, colorbg, colorshadow);
	}



	//////////////////////////////////////////////////////////////////////////////////////////////////////////
	// 
	//		MESSAGE BOX GOOD DESIGNE AND MORE PARAMETER   
	// 
	//////////////////////////////////////////////////////////////////////////////////////////////////////////

	struct sMsgBox {
		// position of windows
		int									x;
		int									y;
		// border decoration
		wchar_t								border[6];
		// string definition
		std::wstring						title;
		std::wstring						body;
		// color
		int									color_title;
		int									color_body;
		int									color_border;
		int									color_background;
	};

	void Message_Box(int x, int y,			// coordinate for box message
		const wchar_t(&_border)[6],		// border chart of table boxing
		std::wstring _title,		    	// title text in message box
		const std::wstring& _body,			// body text 
		int colorfg_title,					// color title
		int colorfg_body,					// color body text
		int colorborder,					// color of line border
		int colorbg							// color general backgound color
	)
	{
		int dy = 6;
		int dx = 4 + static_cast<int>(_body.size());
		box(x, y, dx - 1, dy - 1, 0, colorbg);
		// Corner
		konsole->draw(x - 1, y - 1, _border[0], colorborder | colorbg);
		konsole->draw(x + dx - 1, y - 1, _border[2], colorborder | colorbg);
		konsole->draw(x + dx - 1, y + dy - 1, _border[4], colorborder | colorbg);
		konsole->draw(x - 1, y + dy - 1, _border[5], colorborder | colorbg);
		//// Lines
		//int rep = dx - _title.size() - 3;
		konsole->hdraw(x, y - 1, _border[1], dx - 1, colorborder, colorbg);
		konsole->vdraw(x - 1, y, _border[3], dy - 1, colorborder, colorbg);
		konsole->hdraw(x, y + dy - 1, _border[1], dx - 1, colorborder, colorbg);
		konsole->vdraw(x + dx - 1, y, _border[3], dy - 1, colorborder, colorbg);

		konsole->text_at(x + 1, y - 1, _title, colorfg_title | colorbg);
		konsole->draw(x, y - 1, b_, colorfg_title | colorbg);
		konsole->draw(x + _title.size() + 1, y - 1, _e, colorfg_title | colorbg);

		konsole->text_at(x + 1, y + 2, _body, colorfg_body | colorbg);
	}

	class BuildMessage {
		sMsgBox       msg_box;
	public:

		BuildMessage& set_position(int x, int y) {
			msg_box.x = x;
			msg_box.y = y;
			return *this;
		}

		BuildMessage& set_border(const wchar_t(&border)[6]) {
			std::memcpy(msg_box.border, border, 6 * sizeof(wchar_t));
			return *this;
		}

		BuildMessage& set_title(std::wstring_view title) {
			msg_box.title = title;
			return *this;
		}
		BuildMessage& set_body(std::wstring_view body) {
			msg_box.body = body;
			return *this;
		}

		BuildMessage& set_title_color(int color) {
			msg_box.color_title = color;
			return *this;
		}
		BuildMessage& set_body_color(int color) {
			msg_box.color_body = color;
			return *this;
		}
		BuildMessage& set_border_color(int color) {
			msg_box.color_border = color;
			return *this;
		}
		BuildMessage& set_background_color(int color) {
			msg_box.color_background = color;
			return *this;
		}

		sMsgBox get() const {
			return std::move(msg_box);
		}

	};

	void Message_Box(sMsgBox msg_box) {
		Message_Box(msg_box.x, msg_box.y, msg_box.border, msg_box.title,
			msg_box.body, msg_box.color_title, msg_box.color_body,
			msg_box.color_border, msg_box.color_background);
	}


//////////////////////////////////////////////////////////////////////////////////////////////////////////
// 
//				MENU UTILITY    
// 
//////////////////////////////////////////////////////////////////////////////////////////////////////////
	template<typename T>
		requires std::is_arithmetic_v<T>
	void get_maximum(T& max, const T& var) {
		max = var > max ? var : max;
	}

	template<typename T>
		requires std::is_arithmetic_v<T>
	void get_minimum(T& min, const T& var) {
		min = var < min ? var : min;
	}

	static void draw_string_menu(int x, int y, const std::wstring wstr, int colorfg, int colorbg,
		int color_firstchar) {

		konsole->draw(x, y, wstr[0], color_firstchar | colorbg);
		for (int i = 1; i < wstr.size(); ++i)
			konsole->draw(x + i, y, wstr[i], colorfg | colorbg);
	}


	void SubMenu(int x, int y,						// coordinate where we put bar sub menu
		int step,									// step between items
		const std::vector<std::wstring>& vItems, 	// items in form of string 
		int colorfg, 								// general foreground color
		int colorbg,								// general background color
		int firstCharColor)							// first char color foreground
	{
		// using background black :
		int width = konsole->width();
		int hight = konsole->hight();
		int l{ 1 };

		console::box(x, y, width, 1, colorfg, colorbg);

		for (const auto& str : vItems)
		{
			draw_string_menu(x + l, y, str, colorfg, COLOR::BG_BLACK, firstCharColor);
			l += str.size() + step;
			if (l > width) break;
		}
	}

	template<typename T>
	class VectorBuilder {
		std::vector<T> vec;

	public:

		VectorBuilder& add(const T& item) {
			vec.push_back(item);
			return *this;
		}

		std::vector<T> get() {
			return std::move(vec);
		}
	};

	using strVectorBuilder = VectorBuilder<std::wstring>;


	static void StatuBar(const std::vector<std::wstring>& items) {
		auto Hight = konsole->hight();
		console::SubMenu(0, Hight - 1, 3, items, COLOR::FG_YELLOW, COLOR::FG_DARK_YELLOW << 4, COLOR::FG_RED);

	}


}// namespace console