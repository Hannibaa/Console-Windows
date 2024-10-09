#pragma once
#include <forward_list>
#include <console project/basic/ka_IDrawable.hpp>
#include <console project/basic/ka_Transformable.hpp>
#include <console project/basic/ka_Selectionable.hpp>
#include <include/geometry/rectangle.hpp>
#include <console project/console/ka_utility.hpp>






namespace console {


	//////////////////////////////////////////////////////////////////////////////////////////////////////////
	// 
	//    Interface ISprite // first version
	// 
	//////////////////////////////////////////////////////////////////////////////////////////////////////////
	template<typename T>
	class ISprite : public IDrawable, public Selectionable<T> {

		using value_type = T;
		using TRect = geom2d::Rect<T>;
		using vector_type = geom2d::Vector2d<T>;

	public:

		virtual ISprite* create() = 0;

		virtual TRect get_selectBound() {
			return this->get_bound();
		}

		TRect get_bound() {
			return _rect;
		}

		void set_bound(const TRect& rect) {
			_rect = rect;
		}

		virtual ~ISprite() = default;

	private:

		TRect			_rect;	// maximum allowable view box that can draw
	};

	//////////////////////////////////////////////////////////////////////////////////////////////////////////
	// 
	//    Sprite // first version
	// 
	//////////////////////////////////////////////////////////////////////////////////////////////////////////

	template<typename T>
		requires std::is_arithmetic_v<T>
	class CSprite_view : public geom2d::Rect<T>, IDrawable {

	public:

		CSprite_view()
			:_ptr_shape(nullptr),
			_id(0),
			_size(0),
			_color(COLOR::FG_WHITE),
			_color0(COLOR::FG_WHITE),
			_bg_color(konsole->get_background_color()),
			_bg_color0(konsole->get_background_color()),
			_is_init_fg_color_setted(false),
			_is_init_bg_color_setted(false)
		{
			this->dx = T(1);
		}

		void set_sprite(
			std::wstring* ptr, // pointer to string
			int id,			   // identification number
			int x,			   // position x
			int y,			   // position y
			int dx,			   // size in dx
			int dy,			   // size in dy
			int colorfg,	   // color foreground
			int colorbg	   // color background initial set to console background color
		)
		{
			_ptr_shape = ptr;
			_size = _ptr_shape->size();
			_id = id;
			this->x = x;
			this->y = y;
			this->dx = dx;
			this->dy = dy;
			_bg_color = colorbg;
			_color = colorfg;
			_color0 = _color;
			update_size();
		}

		void set_color(int color) {		// we can set it togather with logical | operation
			_color = color;

			if (!_is_init_fg_color_setted)
			{
				_color0 = _color;
				_is_init_fg_color_setted = true;
			}
		}

		int get_init_fg_color() const noexcept {
			return _color0;
		}

		int get_init_bg_color() const noexcept {
			return _bg_color0;
		}

		void set_bg_color(int color) {
			_bg_color = color;

			if (!_is_init_bg_color_setted) {
				_bg_color0 = _bg_color;
				_is_init_bg_color_setted = true;
			}

		}

		void reset_color_init() {
			_is_init_bg_color_setted = false;
			_is_init_fg_color_setted = false;
		}

		void reset_bg_color() {
			_bg_color = _bg_color0;
		}

		void reset_fg_color() {
			_color = _color0;
		}

		// TODO : make string view fit to rectangle dimension
		void shrink_to_fit() {

		}

		void set_shape(std::wstring* ptr) {
			_ptr_shape = ptr;
			if (_ptr_shape) {
				_size = _ptr_shape->size();
				update_size();
			}
		}

		std::wstring* get_shape() const {
			return _ptr_shape;
		}

		void reset(std::wstring* new_ptr = nullptr) {
			_ptr_shape = new_ptr;
			if (_ptr_shape) {
				_size = _ptr_shape->size();
				update_size();
			}
		}

		virtual geom2d::Vector2d<T> get_position() const override {
			return geom2d::Vector2d<T>(this->x, this->y);
		}

		void set_position(T x, T y) {
			this->x = x;
			this->y = y;
		}

		virtual void set_position(const geom2d::Vector2d<T>& point) override {
			set_position(point.x, point.y);
		}

		geom2d::Rect<T> get_bounds() const {
			return geom2d::Rect<T>(this->x, this->y, this->dx, this->dy);
		}

		void rectangle(T _x, T _y, T _dx, T _dy) {
			this->x = _x;
			this->y = _y;
			this->dx = _dx;
			this->dy = _dy;

			update_size();
		}


		void set_id(size_t id) {
			_id = id;
		}

		size_t get_id() const {
			return _id;
		}

		size_t size() const {
			return _size;
		}

		void draw() const {
			for (int i = 0; i < _size; ++i)
			{
				int k = i % int(this->dx);
				int x = int(this->x) + k;
				int y = int(this->y) + (i - k) / this->dx;
				konsole->draw(x, y, (*_ptr_shape)[i], _bg_color | _color);
			}
		}

	private:

		void update_size() {

			if (this->dx > _size) {
				this->dx = _size;
				this->dy = 1;
				return;
			}

			int i = _size - 1;
			if (this->dx == 0) this->dx = 1;
			this->dy = 1 + T(i - i % int(this->dx)) / this->dx;
		}

		std::wstring* _ptr_shape;
		size_t								_size;
		size_t								_id;
		int									_color;
		int									_bg_color;

		bool								_is_init_fg_color_setted;
		bool								_is_init_bg_color_setted;

		int									_bg_color0;
		int									_color0;

	};
}