/*
			MANAGING SELECTION OBJECT

*/

#pragma once

namespace console{

	template<typename TDrawable>
	class IManagerSelection {
		TDrawable* _ptr{ nullptr };
	public:

		virtual void Register(TDrawable* obj) = 0;
		virtual void Unregister(TDrawable* obj) = 0;

		virtual void Selection(const iVec2& mouse) = 0;
		virtual bool MouseOn(const iVec2& mouse) = 0;

		TDrawable* get() const {
			return _ptr;
		}
	};




}
