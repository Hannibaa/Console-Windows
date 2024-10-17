#pragma once
#include <functional>
#include <console project/basic/ka_Transformable.hpp>
#include <include/geometry/rectangle.hpp>
#include <include/maths/clamp.hpp>






namespace cinematic {

	
//////////////////////////////////////////////////////////////////////////////////////////////////////////
// 
//			KEY CONTROL FOR 
// 
//////////////////////////////////////////////////////////////////////////////////////////////////////////


	class IControle {
	protected:
		short key_up{ VK_UP };
		short key_down{ VK_DOWN };
		short key_right{ VK_RIGHT };
		short key_left{ VK_LEFT };
		short key_fire;
		short key_up_1;
		short key_down_1;

	public:

		void set_controle_direction_key(const short(&controle)[4]) {
			key_up = controle[0];
			key_down = controle[1];
			key_right = controle[2];
			key_left = controle[3];
		}

		void set_auxillary_key(const int(&controle)[3]) {
			key_fire = controle[0];
			key_up_1 = controle[1];
			key_down_1 = controle[2];
		}
	};



//////////////////////////////////////////////////////////////////////////////////////////////////////////
// 
//				INTERFACE OF GENERALE MOVEMENT     
// 
//////////////////////////////////////////////////////////////////////////////////////////////////////////

	class IMove {
	protected:
		fRect				_field;
		fVec2				_speed;

	public:
		virtual void move(fVec2&, float, const fVec2& dim = 0) = 0;

		void set_speed(float vx, float vy) {
			_speed.x = vx;
			_speed.y = vy;
		}

		void set_rectangle(const fRect& rect) {
			_field.x = rect.x;
			_field.y = rect.y;
			_field.dx = rect.dx;
			_field.dy = rect.dy;
		}

		void set_rectangle(float x, float y, float dx, float dy) {
			_field.x  = x;
			_field.y  = y;
			_field.dx = dx;
			_field.dy = dy;
		}

		fRect get_bounds() const {
			return _field;
		}

		void set_speed(fVec2 speed) {
			_speed = speed;
		}

		fVec2 get_speed() const {
			return _speed;
		}

		virtual ~IMove() = default;
	};


//////////////////////////////////////////////////////////////////////////////////////////////////////////
// 
//				INTERFACE OF iMOTION   
// 
//////////////////////////////////////////////////////////////////////////////////////////////////////////

	class IMotion {
	protected:
		fRect				_field;
		fVec2				_speed;

	public:

		virtual void operator()(console::Transformable<float> *, float) = 0;

		void set_speed(float vx, float vy) {
			_speed.x = vx;
			_speed.y = vy;
		}

		void set_speed(fVec2 speed) {
			_speed = speed;
		}

		void set_speed(float speed) {
			_speed.x = 0.7071f * speed;
			_speed.y = 0.7071f * speed;
		}

		void set_rectangle(const fRect& rect) {
			_field.x = rect.x;
			_field.y = rect.y;
			_field.dx = rect.dx;
			_field.dy = rect.dy;
		}

		void set_rectangle(float x, float y, float dx, float dy) {
			_field.x = x;
			_field.y = y;
			_field.dx = dx;
			_field.dy = dy;
		}

		fRect get_bounds() const {
			return _field;
		}


		fVec2 get_speed() const {
			return _speed;
		}

		virtual ~IMotion() = default;
	};


	class I2Motion : public cinematic::IMotion {

	public:
		virtual void move(fVec2& p, float Dt) = 0 {
			if (KeyPressed(VK_UP)) {
				p.y -= _speed.y * Dt;
			}

			if (KeyPressed(VK_LEFT)) {
				p.x -= _speed.x * Dt;
			}

			if (KeyPressed(VK_RIGHT)) {
				p.x += _speed.x * Dt;
			}

			if (KeyPressed(VK_DOWN)) {
				p.y += _speed.y * Dt;
			}
		}

		void operator()(console::Transformable<float>* ptr_, float Dt) override {
			if (ptr_ == nullptr) return;

			fVec2 p = ptr_->get_position();
    		fVec2 dim = ptr_->get_dimension();

			move(p, Dt);

			math::clamp<float>(p.y, 0, konsole->hight() - dim.y);
			math::clamp<float>(p.x, 0, konsole->width() - dim.x);

			ptr_->set_position(p);
		}

	};


//////////////////////////////////////////////////////////////////////////////////////////////////////////
// 
//				STATIC POLYMORPHIC ONE
// 
//////////////////////////////////////////////////////////////////////////////////////////////////////////
	class IMotion2 {
	protected:

		fVec2 _speed;
		fRect _field;

	public:
		virtual void move(fVec2& p, float Dt) = 0;

		// templeted operator
		template<typename TDrawable>
		void operator()(TDrawable* _ptr, float Dt) {
			fVec2 p = _ptr->get_position();

			move(p, Dt);

			_ptr->set_position(p);
		}

		void set_speed(float vx, float vy) {
			_speed.x = vx;
			_speed.y = vy;
		}

		void set_speed(fVec2 speed) {
			_speed = speed;
		}

		void set_speed(float speed) {
			_speed.x = 0.7071f * speed;
			_speed.y = 0.7071f * speed;
		}

		void set_rectangle(const fRect& rect) {
			_field.x = rect.x;
			_field.y = rect.y;
			_field.dx = rect.dx;
			_field.dy = rect.dy;
		}

		void set_rectangle(float x, float y, float dx, float dy) {
			_field.x = x;
			_field.y = y;
			_field.dx = dx;
			_field.dy = dy;
		}

		fRect get_bounds() const {
			return _field;
		}


		fVec2 get_speed() const {
			return _speed;
		}

		virtual ~IMotion2() = default;
	};


	// control
	class Control2 : public cinematic::IMotion2 {
	public:
		void move(fVec2& p, float Dt) override {
			if (KeyPressed(VK_UP)) {
				p.y -= _speed.y * Dt;
			}

			if (KeyPressed(VK_LEFT)) {
				p.x -= _speed.x * Dt;
			}

			if (KeyPressed(VK_RIGHT)) {
				p.x += _speed.x * Dt;
			}

			if (KeyPressed(VK_DOWN)) {
				p.y += _speed.y * Dt;
			}
		}
	};



//////////////////////////////////////////////////////////////////////////////////////////////////////////
// 
//    Generale free motion in term of coordinate x and y
// 
//////////////////////////////////////////////////////////////////////////////////////////////////////////
	class FreeMotion : public IMotion {

		using FunctionReal = std::function<float(float)>;

		FunctionReal  _X;
		FunctionReal  _Y;
		float         _t;

	public:
		FreeMotion(FunctionReal X, FunctionReal Y, float t0 = 0.f)
			: _X(std::move(X))
			, _Y(std::move(Y))
			, _t(t0)
		{}

		void initial_point(float t0) {
			_t = t0;
		}

		
		void operator()(console::Transformable<float>* ptr, float Dt) {
			fVec2  p = ptr->get_position();
			fVec2  dim = ptr->get_dimension();

			_t += _speed.Norm() * Dt;
			p.x = _X(_t);
			p.y = _Y(_t);

			math::clamp<float>(p.y, 0, konsole->hight() - dim.y);
			math::clamp<float>(p.x, 0, konsole->width() - dim.x);

			ptr->set_position(p);

		}
	};

#undef _w
	class RotationMotion : public IMotion {
		fVec2			_center;
		fVec2			_radius;
		float			_angilarVelocity;
		float			_param_speed;
		float			_t;
		const int		_h;
		const int		_w;
		const float		_2PI = 2 * 3.1415f;
	public:
		RotationMotion(const fVec2& center, const fVec2& radius, float angilar_velocity,
			float t0 = 0.f)
			:_center(center)
			, _radius(radius)
			, _angilarVelocity(angilar_velocity)
			, _param_speed(1.f)
			, _t(t0)
			, _h(konsole->hight())
			, _w(konsole->width())
		{}

		void set_radius(float _rx, float _ry) {
			_radius.x = _rx;
			_radius.y = _ry;
		}

		void set_center(float _cx, float _cy) {
			_center.x = _cx;
			_center.y = _cy;
		}

		void operator()(console::Transformable<float>* ptr, float Dt)
		{
			fVec2  p = ptr->get_position();
			fVec2  dim = ptr->get_dimension();

			_t += _speed.Norm() * Dt;
			p.x = _center.x + _radius.x * std::sinf(_2PI * _angilarVelocity * _t) ;
			p.y = _center.y + _radius.y * std::cosf(_2PI * _angilarVelocity * _t) ;

			math::clamp<float>(p.x, 0,_w - dim.x);
			math::clamp<float>(p.y, 0,_h - dim.y);

			ptr->set_position(p);

		}
	};
//////////////////////////////////////////////////////////////////////////////////////////////////////////
// 
//			FREE BOX MOTION     
// 
//////////////////////////////////////////////////////////////////////////////////////////////////////////

	class FreeBoxMotion :public IMotion {
		float		v1{1.f};
		float		v2{1.f};

		enum REVERSE_CASE {
			REV_V1V2 = 0,
			REV_V1 = 1,
			REV_V2 = 2,
		};
	public:
		void operator()(console::Transformable<float>* ptr, float Dt)
		{
			fVec2  p = ptr->get_position();
			fVec2  dim = ptr->get_dimension();

			p.x += _speed.x * Dt * v1;
			p.y += _speed.y * Dt * v2;

			if (p.x < _field.x || p.x > _field.x + _field.dx - dim.x + 2.f) v1 *= -1.f;
			if (p.y < _field.y || p.y > _field.y + _field.dy - dim.y + 2.f) v2 *= -1.f;

			ptr->set_position(p);
		}

		void reverse(int _case = 0) {
			switch (_case) {
			case REVERSE_CASE::REV_V1V2:
				v1 = -v1;
				v2 = -v2;
				break;
			case REVERSE_CASE::REV_V1:
				v1 = -v1;
				break;
			case REVERSE_CASE::REV_V2:
				v2 = -v2;
				break;
			default:
				break;
			}

		}
	};


	class FreeBoxMotion2 :public IMotion2 {
		float		v1{ 1.f };
		float		v2{ 1.f };
		fVec2		dim;

		enum REVERSE_CASE {
			REV_V1V2 = 0,
			REV_V1 = 1,
			REV_V2 = 2,
		};
	public:
		void set_dimension(float dx, float dy) {
			dim.x = dx;
			dim.y = dy;
		}

		void move(fVec2& p, float Dt) override {

			p.x += _speed.x * Dt * v1;
			p.y += _speed.y * Dt * v2;

			if (p.x < _field.x || p.x > _field.x + _field.dx - dim.x + 2.f) v1 *= -1.f;
			if (p.y < _field.y || p.y > _field.y + _field.dy - dim.y + 2.f) v2 *= -1.f;
		}

		void reverse(int _case = 0) {
			switch (_case) {
			case REVERSE_CASE::REV_V1V2:
				v1 = -v1;
				v2 = -v2;
				break;
			case REVERSE_CASE::REV_V1:
				v1 = -v1;
				break;
			case REVERSE_CASE::REV_V2:
				v2 = -v2;
				break;
			default:
				break;
			}

		}
	};

}// namespace cinematic