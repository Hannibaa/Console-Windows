#pragma once
#include <include/geometry/rectangle.hpp>
#include <include/Macros.hpp>



namespace console {

	template<typename T>
	class ITransformable {

	public:
		using value_type = T;
		using vector_type = geom2d::Vector2d<value_type>;
		using rect_type = geom2d::Rect<value_type>;

		virtual ~ITransformable() = default;

		// this function return dimension volume in two value.
		virtual vector_type get_dimension() const = 0;
		virtual rect_type	get_bounds() const = 0;

		virtual void set_position(value_type x, value_type y)
		{
			_pos.x = x, _pos.y = y;
		}

		virtual void set_position(vector_type const& position) {
			_pos = position;
		}

		vector_type get_position() const {
			return _pos;
		}

		void set_origine(value_type x, value_type y) {
			_origine.x = x;
			_origine.y = y;
		}

		void set_origine(vector_type const& origine) {
			_origine = origine;
		}

		vector_type get_origine() const {
			return _origine;
		}

		virtual void move(value_type offsetX, value_type offsetY) {
			_pos.x += offsetX;
			_pos.y += offsetY;
		}

		void set_rect(const rect_type& rect) {
			_rect = rect;
		}

		rect_type get_rect() const {
			return _rect;
		}

	private:

		vector_type		_pos;
		vector_type		_origine;
		rect_type		_rect;
	};


}

