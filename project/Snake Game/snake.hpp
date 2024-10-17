#pragma once
#include <array>
#include <vector>
#include <list>
#include <functional>
#include <console project/console/ka_utility.hpp>
#include <console project/basic/ka_ICinematics.hpp>
#include <console project/component/ka_ImageString.hpp>
#include <include/random/random_variable.h>

#define FUNC_SELECTION_1

namespace snake {
	// make random element
	template<typename ...Ts>
	wstringImg make_One(const Ts&...args);
	// make vector of wstringImg.
	template<typename...Ts>
	std::vector<wstringImg> make_Multi(const Ts...args);
	
	// make container of object type
	template<template<typename...> class Container,typename TDRAWABLE, typename ...Ts>
	Container<TDRAWABLE> make_Container(const Ts&...args);


	class Snake : public console::IDrawable {
		// types
	public:
		using value_type		= wstringImg;
		using container_type	= std::vector<value_type>;

	private:
		container_type						_X;
		size_t								_size;

	public:
		Snake(const container_type& _arr, const fVec2& p0)
			:_X(_arr)
		{
			_size = _arr.size();
			init_position(p0);
		}

		Snake(const fVec2& p0) {
			auto _X0 = make_One();
			_X.push_back(_X0);
			_X[0].set_position(p0);
			_size = _X.size();
		}

		void set_position(const fVec2& p) {
			fVec2 _old_p = _X[0].get_position();
			_X[0].set_position(p);

			for (int i = 1; i < _size; ++i) {
				fVec2 temp_p = _X[i].get_position();
				if (!physic::is_collid(_X[i], _X[i - 1])) {
					_X[i].set_position(_old_p);
					_old_p = temp_p;
				}
			}
		}

		void resize(size_t size) {
			_X.resize(size);
			_size = size;
		}

		void set_color(int colors) {
			for (auto& x : _X) x.color_bg<int>(colors);
		}

		bool is_headCollid() {
			if (_size > 1)
			{
				for (int i = 1; i < _size; ++i) {
					if (physic::is_collid(_X[0], _X[i])) {
						return true;
					}
				}
			}

			return false;
		}


		void set_position(float x, float y) {
			set_position(fVec2(x, y));
		}
		
		fRect get_bounds() const {
			return _X[0].get_bounds();
		}

		fVec2 get_position() const {
			return _X[0].get_position();
		}

		constexpr size_t get_size() const {
			return _X.size();
		}

		void add(const value_type& value) {
			_X.push_back(value);
			_size = _X.size();
		}

		void pop() {
			_X.pop_back();
			_size = _X.size();
		}

		std::wstring string() const {
			std::wstring str;
			for (const auto& x : _X) str.append(x.string());

			return str;
		}

		void draw() const override {
			for (const auto& S : _X)
				S.draw();
		}

	private:
		void init(const container_type& _arrString)
		{
			if constexpr (std::is_same_v<container_type, std::vector<value_type>> || 
				std::is_same_v<container_type, std::list<value_type>> )
			_X.resize(_arrString.size());

			for (int i = 0; i < _arrString.size(); ++i)
				_X[i] = _arrString[i];
		}

		// inialization of position
		void init_position(const fVec2& p0) {
			_X[0].set_position(p0);
			for (int i = 1; i < _size; ++i)
			{
				fVec2 pk = _X[i - 1].get_position() + _X[i - 1].get_dimension();
				_X[i].set_position(pk);
			}
		}
	};

}



namespace snake {
//////////////////////////////////////////////////////////////////////////////////////////////////////////
// 
//		CREATE ARRAY OF BOX    
// 
//////////////////////////////////////////////////////////////////////////////////////////////////////////
	template<size_t N>
	std::array<wstringImg, N>  make_snake(int lx, int ly) {
		RV::RVec<int> rcolor(color::Red, color::Green, color::Black);

		std::array<wstringImg, N>    _arr;

		for (int i = 0; i < N; ++i)
			_arr[i] = strImg::make_StringImage(2, 2, rcolor());
		return  _arr;
	}


//////////////////////////////////////////////////////////////////////////////////////////////////////////
// 
//     STATIC SNAKE CLASS
// 
//////////////////////////////////////////////////////////////////////////////////////////////////////////
	template<size_t N>
	class StaticSnake : public console::IDrawable {
	public:
		using value_type = wstringImg;
		using container_type = std::array<value_type, N>;
	private:

		container_type						_X;
		size_t								_size{ N };

	public:
		StaticSnake(const container_type& _arr, const fVec2& p0)
			:_X(_arr)
		{
			init_position(p0);
		}

		void set_position(const fVec2& p) {
			fVec2 _old_p = _X[0].get_position();
			_X[0].set_position(p);

			for (int i = 1; i < _size; ++i) {
				fVec2 temp_p = _X[i].get_position();
				if (!physic::is_collid(_X[i], _X[i - 1])) {
					_X[i].set_position(_old_p);
					_old_p = temp_p;
				}
			}
		}

		void set_position(float x, float y) {
			set_position(fVec2(x, y));
		}

		fVec2 get_position() const {
			return _X[0].get_position();
		}

		constexpr size_t get_size() const {
			return _size;
		}

		bool is_headCollid() {
			if (_size > 3)
			{
				for (int i = 3; i < _size; ++i) {
					if (physic::is_collid(_X[0], _X[i])) return true;
				}
			}

			return false;
		}

		void draw() const override {
			for (const auto& S : _X)
				S.draw();
		}

	private:
		void init(const container_type& _arrString)
		{
			for (int i = 0; i < N; ++i)
				_X[i] = _arrString[i];
		}

		// inialization of position
		void init_position(const fVec2& p0) {
			_X[0].set_position(p0);
			for (int i = 1; i < _size; ++i)
			{
				fVec2 pk = _X[i - 1].get_position() + _X[i - 1].get_dimension();
				_X[i].set_position(pk);
			}
		}
	};



//////////////////////////////////////////////////////////////////////////////////////////////////////////
// 
//    control snake
// 
//////////////////////////////////////////////////////////////////////////////////////////////////////////
	
	namespace control {

		class SnakeControl : public cinematic::IMotion2 {
			bool b_up{ false };
			bool b_left{ false };
			bool b_right{ true };
			bool b_down{ false };
			bool b_noreturn{ false };

			void all_false() {
				b_up = false;
				b_left = false;
				b_right = false;
				b_down = false;
			}

			short _left{ VK_LEFT }, _down{ VK_DOWN }, _right{ VK_RIGHT }, _up{ VK_UP };
			short _sequeez{ _u('H') };
			const short _direction[4]{_left, _up, _right, _down};
			int		_active{2};

		public:
			void set_control(char left, char down, char right, char up) {
				_left = _u(left);
				_down = _u(down);
				_right = _u(right);
				_up = _u(up);
			}

			void noreturn(bool b) {
				b_noreturn = b;
			}

			void sequeez(char s) {
				_sequeez = _u(s);
			}

			void sequeez() {
				if (b_left) b_left = false;
				if (b_right) b_right = false;
				if (b_up) b_up = false;
				if (b_down) b_down = false;
			}

			short get_direction() const {
				return _direction[_active];
			}

			void move(fVec2& p, float Dt) 
			{
				if (KeyReleased(_sequeez)) {
					all_false();
				}

				if (KeyReleased(_up)) {
					if (b_noreturn && b_down );
					else
					{
						all_false();
						b_up = true;
						_active = 1;
					}
				}

				if (KeyReleased(_left)) {
					if (b_noreturn && b_right );
					else
					{
						all_false();
						b_left = true;
						_active = 0;
					}
				}

				if (KeyReleased(_right)) {
					if (b_noreturn && b_left );
					else
					{
						all_false();
						b_right = true;
						_active = 2;
					}
				}

				if (KeyReleased(_down)) {
					if (b_noreturn && b_up );
					else
					{
						all_false();
						b_down = true;
						_active = 3;
					}
				}

				if (b_up) {
					p.y -= _speed.y * Dt;
				}

				if (b_left) {
					p.x -= _speed.x * Dt;
				}

				if (b_right) {
					p.x += _speed.x * Dt;
				}

				if (b_down) {
					p.y += _speed.y * Dt;
				}

				math::clamp<float>(p.x, 0.f, konsole->width());
				math::clamp<float>(p.y, 0.f, konsole->hight());
			}
		};


		
		class SnakeFreeMotion : public cinematic::IMotion2 {
			bool b_up{ false };
			bool b_left{ false };
			bool b_right{ true };
			bool b_down{ false };

			void all_false() {
				b_up = false;
				b_left = false;
				b_right = false;
				b_down = false;
			}

			rng::fRG<float> frand;

			float xmin, xmax, ymin, ymax;
			float step;
			int N;

		public:
			SnakeFreeMotion(float xmin, float xmax, float ymin, float ymax, int N)
				:xmin(xmin)
				,xmax(xmax)
				,ymin(ymin)
				,ymax(ymax)
				,N(N)
			{}

			void move(fVec2& p, float Dt)
			{
				step += _speed.Norm() * Dt;

				if ((int(step)) % N == 0) {

					if (b_up || b_down) {
						all_false();
						b_left = true;
					}
				}
				
				if ((int(step)) % N == N-1) {

					if (b_right || b_left) {
						all_false();
						b_down;
					}
				}

				if (b_up) {
					p.y -= _speed.y * Dt;
				}

				if (b_left) {
					p.x -= _speed.x * Dt;
				}

				if (b_right) {
					p.x += _speed.x * Dt;
				}

				if (b_down) {
					p.y += _speed.y * Dt;
				}

				if (p.x > xmax) {
					
					all_false();
					b_left = true;
				}

				if (p.x < xmin) {
					all_false();
					b_right = true;
				}

				if (p.y > ymax) {
					all_false();
					b_down = true;
				}

				if (p.y < ymin) {
					all_false();
					b_up = true;
				}

				math::clamp<float>(p.x, 0.f, konsole->width());
				math::clamp<float>(p.y, 0.f, konsole->hight());
			}

		};
	}



//////////////////////////////////////////////////////////////////////////////////////////////////////////
// 
//    definition of make function
//    if also define by macor selection
//////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifdef  FUNC_SELECTION_
	template<typename ...Ts>
	wstringImg make_One(const Ts&...) {
		rng::RG<char>  wrand('A', 'Z');
		std::wstring w(1, static_cast<wchar_t>(wrand()));
		RV::RVec<int>   col{ color::Red, color::Green, color::Black , color::Yellow, color::Magenta };

		return strImg::make_StringImage(w, 1,1,col());
	}

	std::vector<wstringImg> make_Multi(size_t _size) {
		std::vector<wstringImg>  vstr(_size);

		for (auto& e : vstr) e = make_One();

		return vstr;
	}
#endif  // FUNC 1

#ifdef FUNC_SELECTION_2
	template<typename ...Ts>
	wstringImg make_One(const Ts&...) {
		rng::RG<char>  wrand('A', 'Z');
		std::wstring w(1, static_cast<wchar_t>(wrand()));
		RV::RVec<int>   col{ color::Red, color::Green, color::Black , color::Yellow, color::Magenta };

		return strImg::make_StringImage(w, 1, 1, col());
	}

#endif // FUNC 2
}