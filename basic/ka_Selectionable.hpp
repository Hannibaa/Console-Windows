#pragma once
#include <functional>
#include <console project/basic/ka_Transformable.hpp>
#include <include/geometry/rectangle.hpp>
#include <include/Macros.hpp>





namespace console {

	template<typename TRect>
	class IISelectionable {
	public:
		virtual TRect get_selectionArea() const = 0;

		virtual ~IISelectionable() = default;
	};


	template<typename T>
	REQUIRES_ARTH(T)
	class Selectionable  : public geom2d::Rect<T> {
		using value_type  = T;
		using TRect = geom2d::Rect<T>;
		using vector_type = geom2d::Vector2d<T>;
		using Function    = std::function<bool()>;
	public:

		using geom2d::Rect<T>::Rect;

		TRect get_bound() const {
			return TRect(this->x, this->y, this->dx, this->dy);
		}

		void set_action(Function action) {
			_action = action;
		}

		void set_selectColor(int color) {
			_color_sel = color;
		}

		int get_selectColor() const {
			return _color_sel;
		}

		void set_actionColor(int color) {
			_color_action = color;
		}

		int get_actionColor(int color) {
			return _color_action;
		}

		bool selected(const vector_type& mouse) {
			_is_selected = false;

			if (this->contain(mouse)) _is_selected = true;
				 
			return _is_selected;
		}

		vector_type get_offset(const vector_type& mouse) {
			vector_type voffset{};
			if (_action())
				if (_is_selected && !_is_draging)
					voffset = this->get_position() - mouse;

			return voffset;
		}

		bool dragging(const vector_type& mouse) {

			if (_action())
			{
				_is_clicked = true;
				if (_is_selected && !_is_draging)
				{
					_is_draging = true;
					_rect_offset = this->get_position() - mouse;
				}

			}
			else
			{
				_is_draging = false;
				_is_clicked = false;
			}

			return _is_draging;
		}

		void drag(const vector_type& mouse)
		{
			if (_is_draging )
			{
				this->set_position(mouse + _rect_offset);
			}
		}

		virtual ~Selectionable() = default;

	private:

		int					_color_sel;
		int					_color_action;
		vector_type			_rect_offset;

		bool				_is_selected{ false };
		bool				_is_draging{false};
		bool				_is_clicked{ false };

		Function			_action;
	};

}