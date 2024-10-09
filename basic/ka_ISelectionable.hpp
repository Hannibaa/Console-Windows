#pragma once
#include <include/Macros.hpp>


namespace console {

	template<typename TRect, typename Transformable, typename Vector>
	class ISelectionable {

		Transformable&		obj;
		TRect				rect;
	public:

		ISelectionable(Transformable& _obj)
			:obj(_obj)
		{}

		TRect get_selectArea() const {
			return rect;
		}

		virtual bool MouseOn(const Vector& mouse) = 0;
		virtual bool MouseAction(const Vector& mouse) = 0;

		virtual ~ISelectionable() = default;
	};


}