#pragma once
#include <forward_list>
#include <variant>
#include <console project/basic/ka_IDrawable.hpp>
#include <console project/basic/ka_Transformable.hpp>
#include <console project/basic/ka_Selectionable.hpp>
#include <include/geometry/rectangle.hpp>
#include <console project/console/ka_utility.hpp>






namespace console {


	template<typename TDrawable, typename T>
	class PackDrawable : public TDrawable {
	public:
		using TVector = geom2d::Vector2d<T>;
		using TRect = geom2d::Rect<T>;
		using type_object = std::pair<TDrawable*, Selectionable<T>>;
		using type_list = std::forward_list<type_object>;
		using type_select = Selectionable<T>;
		using type_function = std::function<bool()>;

		PackDrawable() = default;

		virtual ~PackDrawable() = default;

		void set_action(type_function action) {
			_action = action;
		}

		void registerDrawable(TDrawable* pdraw, const TRect& selection_rect) {
			type_select  select;
			select.set_rectangle(selection_rect);
			select.set_action(_action);

			_list.push_front(type_object(pdraw, select));
		}

		void removeDrawable(TDrawable* pdraw) {
			_list.remove(pdraw);
		}

		void swap_front(TDrawable* pdraw) {
			_list.remove(pdraw);
			_list.push_front(pdraw);
		}

		virtual void draw() const override {
			for (const auto& e : _list)
				e.first->draw();
		}

		bool is_selected(const geom2d::Vector2d<T>& mouse) {

			for (auto& e : _list)
			{
				if (e.second.selected(mouse)) {
					_ptr = &e;
					return true;
				}
			}

			_ptr = nullptr;

			return false;
		}

		type_object* get_selected() const {
			return _ptr;
		}

		void drag(const TVector& mouse)
		{
			if (is_selected(mouse))
			{
				TVector offset = _ptr->first->get_position() - _ptr->second.get_position();
				if (_ptr->second.dragging(mouse)) {

					_ptr->second.drag(mouse);
					_ptr->first->set_position(_ptr->second.get_position());

				}
			}
		}

	private:

		void update(type_object& item, const TVector& mouse) {
			
			TVector offset = item.first->get_position() - item.second.get_position();

			item.first->set_position(offset + mouse);

		}

		type_function					_action;
		type_list						_list;
		type_object *					_ptr{nullptr};
	};

}