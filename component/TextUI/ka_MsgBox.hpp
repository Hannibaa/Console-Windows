#pragma once

//				All Messages Boxes will edited here
#include <string_view>
#include <console project/component/ka_Draw.hpp>






namespace TextUI {

	//////////////////////////////////////////////////////////////////////////////////////////////////////////
	// 
	//		MESSAGE BOX THAT HAVE BUTTON    
	// 
	//////////////////////////////////////////////////////////////////////////////////////////////////////////

	void messageBox(const iRect& rbox, const std::wstring& wmsg, int colorfg = COLOR::FG_WHITE,
		int colorbg = COLOR::BG_BLUE,
		int colorshadow = COLOR::BG_DARK_GREY)
	{
		Draw::box(rbox, colorfg, colorbg);

		if (colorshadow != -1)
			Draw::shadow(rbox, colorshadow);

		konsole->text_at(rbox.x + 1, rbox.y + 1, wmsg, colorfg | colorbg);
	}

	void messageBox(int x, int y, 
		const std::wstring&				wmsg,
		int								colorfg = COLOR::FG_WHITE,
		int								colorbg = COLOR::BG_BLUE,
		int								colorshadow = COLOR::BG_DARK_GREY)
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
		const wchar_t(&_border)[6],			// border chart of table boxing
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
		Draw::box(x, y, dx - 1, dy - 1, 0, colorbg);
		// Corner
		konsole->draw(x - 1, y - 1, _border[0], colorborder | colorbg);
		konsole->draw(x + dx - 1, y - 1, _border[2], colorborder | colorbg);
		konsole->draw(x + dx - 1, y + dy - 1, _border[4], colorborder | colorbg);
		konsole->draw(x - 1, y + dy - 1, _border[5], colorborder | colorbg);
		//// Lines
		//int rep = dx - _title.size() - 3;
		Draw::hdraw(x, y - 1, _border[1], dx - 1, colorborder, colorbg);
		Draw::vdraw(x - 1, y, _border[3], dy - 1, colorborder, colorbg);
		Draw::hdraw(x, y + dy - 1, _border[1], dx - 1, colorborder, colorbg);
		Draw::vdraw(x + dx - 1, y, _border[3], dy - 1, colorborder, colorbg);

		konsole->text_at(x + 1, y - 1, _title, colorfg_title | colorbg);
		konsole->draw(x, y - 1, console::b_, colorfg_title | colorbg);
		konsole->draw(x + _title.size() + 1, y - 1, console::_e, colorfg_title | colorbg);

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




}// End namespace Msg