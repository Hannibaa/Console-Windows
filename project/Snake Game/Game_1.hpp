#pragma once
#include <console project/component/utils.hpp>
#include <console project/component/TextUI/ka_StatuBar.hpp>
#include <console project/component/TextUI/ka_MsgBox.hpp>
#include <console project/project/Snake Game/snake.hpp>
#include <include/random/random_variable.h>
#include <console project/basic/ka_types.hpp>
#include <console project/component/ka_SString.hpp>

#define _tow(str)			std::to_wstring(str)

namespace snake {

	template<typename ...Ts>
	wstringImg make_One(const Ts&...) {
		int w = konsole->width();
		int h = konsole->hight();

		RV::RVec<int>   col{ color::Red, color::Green, color::Black , color::Yellow, color::Magenta };

		return strImg::make_StringImage(std::wstring(2 * 2, L' '), 2, 2, col(),
			1.f, float(w), 1.f, float(h));
	}

	template<>
	wstringImg make_One<int, int>(const int& lx, const int& ly) {
		int w = konsole->width();
		int h = konsole->hight();
		RV::RVec<int>   col{ color::Red, color::Green, color::Black , color::Yellow, color::Magenta };
		return strImg::make_StringImage(std::wstring(lx * ly, L' '), lx, ly, col(),
			1.f, float(w), 1.f, float(h));
	}

	template<>
	std::vector<wstringImg> make_Multi<size_t>(const size_t size) {
		std::vector<wstringImg> vstr(size);

		for (auto& e : vstr) e = make_One(2, 2);

		return vstr;
	}
}

static void grid(const fVec2& p, int wide, int high) {
	Draw::box(0, p.y, wide, 1, color::Black, color::BlueLight << 4);
	Draw::box(p.x, 0, 1, high, color::Black, color::BlueLight << 4);
}

template<typename GeometricObj, typename MOTION>
static void target_line1(const GeometricObj& X, const MOTION& motion, int wide, int high) {
	fVec2 p = X.get_position();
	int  d = motion.get_direction();
	switch (d) {
	case VK_DOWN: Draw::box(p.x, p.y, 1, high - p.y, color::Black, color::GrayLight);
		break;
	case VK_RIGHT: Draw::box(p.x, p.y, wide - p.x, 1, color::Black, COLOR::BG_GREY);
		break;
	case VK_UP: Draw::box(p.x, 0, 1, p.y, color::Black, color::GrayLight);
		break;
	case VK_LEFT: Draw::box(0, p.y, p.x, 1, color::Black, color::GrayLight);
		break;
	}
}

template<typename GeometricObj, typename MOTION>
static void target_line(const GeometricObj& X, const MOTION& motion, int wide, int high) {
	fVec2 p = X.get_position();
	int  d = motion.get_direction();
	int cbg = konsole->get_background_color();
	switch (d) {
	case VK_DOWN: Draw::vdraw(p.x, p.y, L'|', high - p.y, color::Black, cbg);
		break;
	case VK_RIGHT: Draw::hdraw(p.x, p.y, L'-', wide - p.x, color::Black, cbg);
		break;
	case VK_UP: Draw::vdraw(p.x, 0, L'|', p.y, color::Black, cbg);
		break;
	case VK_LEFT: Draw::hdraw(0, p.y, L'-', p.x, color::Black, cbg);
		break;
	}
}

static void register_name(WString& name1)
{
	std::cout << "Snake Game Version 1.0.0\n";
	std::cout << "Kadda Aoues Software Design\n";
	std::cout << "Two Player : " << end_;
	std::cout << "Name 1 : ";
	std::wcin >> name1;
}

template<typename Xrandom, typename IntRandom, typename Stratigy>
void collision(snake::Snake& _snake, wstringImg& food, Stratigy& stratigy,
	Xrandom& x, Xrandom& y, IntRandom& col) {
	if (physic::is_collid(_snake, food)) {
		_snake.add(food);
		stratigy(food, x(), y(), col());
	}
}

template<template<typename...>class Container>
void collision(snake::Snake& _snake, Container<wstringImg>& vfoods, bool& condition) {
	for (auto it = vfoods.begin(); it != vfoods.end(); ) {
		if (physic::is_collid(_snake, *it)) {
			it->set_position(0.f, 0.f);
			_snake.add(*it);
			condition = false;
			it = vfoods.erase(it);
		}
		else
			++it;
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////
// 
//    save stat in file
// 
//////////////////////////////////////////////////////////////////////////////////////////////////////////

static void save_stat() {
	// 1. save sprite image
	// 2. save position and size of vector
	// 3. save color and size of window
	// 4. save option 
}

static int Game() {

	WString   player{ L"karim" };


	const int cwidth = 140;
	const int cheight = 35;
	const int cwfont = 12;
	const int chfont = 12;
	const int index = 2;
	const float Velocity = 0.6f;

	int MINIMUM_FOODS{ 2 };
	int NUMBER_FOODS{ 3 };
	int Max_Game_Score = 20;
	int snake_color{ 1 };

	fVec2 v0{ 0.32f, 0.22f };
	fVec2 p0{ 10.f, 10.f };

	bool b_constrain_direction{ false };
	bool b_set_color{ false };

	bool b_condition{ false };

	WSString   notify;



	// construct window console
	konsole->construct_console(cwidth, cheight, cwfont, chfont);
	// set drawable window
	konsole->set_drawWindows(0.f, 0.f, cwidth, cheight - index);
	// set background color
	konsole->set_background_color(color::Blue << 4);

	fRect window(0.f, 0.f, cwidth, cheight - index);



	/////////////////////////////// Designe Snake /////////



	// andd new snake
	snake::Snake    _snake(p0);


	auto stratigy = [](wstringImg& food, float x, float y, int col) {
		food.set_position(x, y);
		food.color_bg(col);
		};



	rng::RG<int>  x(2, cwidth - 3);
	rng::RG<int>  y(2, cheight - 3);
	RV::RVec<int> c{ color::Red,color::Yellow, color::Green, color::Yellow };

	//////////////////////////////// Create Motion ////////

	cinematic::IMotion* ptr_motion{ nullptr };


	////////////////////////////////// Controled motion

	snake::control::SnakeControl scontrol;
	scontrol.set_rectangle(window);
	scontrol.set_speed(v0);


	std::list<snake::Snake::value_type>						vfoods;
	//snake::Snake::container_type							vfoods;


	//////////////////////////////////////////////////////////////////////////////////////////////////////////
	// 
	//			MAIN LOOP    
	// 
	//////////////////////////////////////////////////////////////////////////////////////////////////////////
	Time::Game_Timer timer;
	int count{};
	int minutes{};

	bool b_move = false;
	bool b_grid{ false };

	//auto vStatuBar1 = ;
	auto vStatuBar2 = TextUI::strVectorBuilder{}
		.add(L"\x25ba RIGHT")
		.add(L"\x25c4 LEFT")
		.add(L"\x25b2 UP")
		.add(L"\x25bc DOWN")
		.add(L"E Exit")
		.add(L"S Squeez")
		.add(L"C+UProw +speed")
		.add(L"C+DOWNrow -speed")
		.add(L"G e(de)nable Grid")
		.add(L"R Reset")
		.get();

	fVec2 mouse{};

	///////////////////////////////////////// LOOP //////////////////////////////////////////////////////
	while (konsole->is_open()) {
		konsole->clear();

		KA_MAKE_FPS(timer);
		KA_MAKE_DALTA_TIME(1);
		minutes = static_cast<int>(timer.time_since_begin());

		while (konsole->pull_event()) {
			++count;
			mouse = fVec2(konsole->get_mouseXY());
		}

		iVec2 snake_coord = _snake.get_position().as<int>();

		PRINT_TITLE(L"Snake Application -- Kadda Aoues fps[%4.2f] minutes[%d] Mouse[%d|%d]",
			ka_fps, minutes, int(mouse.x), int(mouse.y));




		////////////////////////////////////// COMMAND ////////////////////////////////////////
		if (vfoods.size() < MINIMUM_FOODS) {
			auto v = snake::make_Multi<size_t>(NUMBER_FOODS);
			vfoods.insert(vfoods.begin(), v.begin(), v.end());
		}

		// active ghost grid
		if (KeyReleased(_u('G'))) {
			b_grid = !b_grid;
		}

		// reset 
		if (KeyReleased(_u('R'))) {
			_snake.resize(1);
			vfoods.resize(1);
		}

		//constain on direction
		if (KeyReleased(_u('V'))) {
			b_constrain_direction = !b_constrain_direction;
			scontrol.noreturn(b_constrain_direction);
		}

		// Control speed of snake
		if (KeyPressed(VK_CONTROL)) {
			if (KeyReleased(VK_DOWN)) {
				auto v = scontrol.get_speed();
				v.x -= 0.01;
				v.y -= 0.01;
				scontrol.set_speed(v);
			}

			if (KeyReleased(VK_UP)) {
				auto v = scontrol.get_speed();
				v.x += 0.01;
				v.y += 0.01;
				scontrol.set_speed(v);
			}
		}

		if (KeyReleased(_u('C'))) {
			_snake.set_color((++snake_color) % 16);
		}

		/////////////////////////////////////// DYNAMIC & COLLISION ////////////////////////////////

		scontrol(&_snake, ka_Dt);

		if (_snake.is_headCollid()) {
			b_condition = true;
		}

		if (b_condition) {
			scontrol.sequeez();
		}

		collision<std::list>(_snake, vfoods, b_condition);

		// If hite the wall than became resized to 1
		auto sp = _snake.get_position();
		if (sp.x > cwidth - 2 || sp.y > cheight - index || sp.x < 0.5f || sp.y < 0.5f)
		{
			notify = L"Hite The Wall";
		}
		else
			notify = L"Inside Box";


		// set grid 
		if (b_grid) {
			target_line(_snake, scontrol, cwidth, cheight);
		}

		// first statu bar
		TextUI::StatuBar(TextUI::strVectorBuilder{}
			.add(L"Mouse " + std::to_wstring(int(mouse.x)) + L" " + std::to_wstring(int(mouse.y)))
			.add(L"Date " + console::make_daytime_wstring())
			.add(L"Event Count " + std::to_wstring(count))
			.add(L"Snake " + _tow(snake_coord.x) + L"|" + _tow(snake_coord.y))
			.add(L"Color : " + _tow(snake_color))
			.add(player + L" [" + _tow(_snake.get_size()) + L"]")
			.add(notify)
			.get(), 2);
		// second statu bar
		TextUI::StatuBar(vStatuBar2);

		for (const auto& f : vfoods) f.draw();

		_snake.draw();

		konsole->display();
	}





	return 0;
}


