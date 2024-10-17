#pragma once
#include <include/geometry/rectangle.hpp>
#include <include/Macros.hpp>



namespace console {


	template<typename T>
	REQUIRES_ARTH(T)
	class Transformable {

		using value_type = T;
		using vector_type = geom2d::Vector2d<T>;

	public:

		virtual ~Transformable() = default;

		virtual void set_position(T x, T y)
		{
			_pos.x = x, _pos.y = y;
		}

		virtual void set_position(vector_type const& position) {
			_pos = position;
		}

		vector_type get_position() const {
			return _pos;
		}

		void set_origine(T x, T y) {
			_origine.x = x;
			_origine.y = y;
		}

		void set_origine(vector_type const& origine) {
			_origine = origine;
		}

		vector_type get_origine() const {
			return _origine;
		}

		// this function return dimension volume in two value.
		virtual vector_type get_dimension() const {
			return {};
		}

		template<typename U>
		REQUIRES_ARTH(U)
		void move(U offsetX , U offsetY) {
			_pos.x += static_cast<T>(offsetX);
			_pos.y += static_cast<T>(offsetY);
		}

	private:

		vector_type   _pos;
		vector_type   _origine;

	};


}