#pragma once
#include <variant>
#include <console project/console/ka_utility.hpp>
#include <console project/component/ka_ColorString.hpp>
#include <console project/component/ka_ICinematics.hpp>
#include <console project/component/ka_ISprite.hpp>
#include <console project/component/ka_ImageString.hpp>
#include <include/time/Game_timer.h>
#include <console project/component/ka_Physics.hpp>
#include <include/random/random_generator.h>



namespace utils {


	class Jump :public cinematic::IMotion {

		float _intensity{ 1.7f };
		float _ground{ 35.f };
	public:

		void operator()(console::Transformable<float>* ptr_, float Dt) override {
			if (ptr_ == nullptr) return;

			fVec2 p = ptr_->get_position();
			fVec2 dim = ptr_->get_dimension();

			if (KeyPressed(VK_UP)) {
				p.y -= _intensity * _speed.x * Dt;
			}

			if (KeyPressed(VK_LEFT)) {
				p.x -= _speed.x * Dt;
			}

			if (KeyPressed(VK_RIGHT)) {
				p.x += _speed.x * Dt;
			}

			p.y += _speed.x * Dt;

			math::clamp<float>(p.y, 0, _ground - dim.y);
			math::clamp<float>(p.x, 0, 145 - dim.x);

			ptr_->set_position(p);
		}
	};

	class Control : public cinematic::I2Motion {

	public:
		void move(fVec2& p, float Dt) override {
			if (KeyPressed(VK_UP)) {
				p.y -= _speed.y * Dt;
				//Beep(750, 500);                    // beep that block execution not need
			}

			if (KeyPressed(VK_LEFT)) {
				p.x -= _speed.x * Dt;
				//Beep(500, 1000);
			}

			if (KeyPressed(VK_RIGHT)) {
				p.x += _speed.x * Dt;
			}

			if (KeyPressed(VK_DOWN)) {
				p.y += _speed.y * Dt;
			}
		}

	};

	template<typename TShape>
	class Pack {
		std::vector<TShape*>  _shapes;

	public:
		Pack() {
			_shapes.reserve(100);
		}

		void add(TShape* s) {
			_shapes.push_back(s);
		}

		size_t size() const {
			return _shapes.size();
		}

		TShape* at(size_t i) {
			if (i < _shapes.size())
				return _shapes.at(i);
		}

		void manage_collision(float r = 2.0f) {
			for (auto& s1 : _shapes)
				for (auto& s2 : _shapes)
				{
					if (physic::is_collid(*s1, *s2)) {

						physic::collision_process(*s1, *s2, r);
						break;
					}


				}
		}

		void draw() const {
			for (const auto& s : _shapes) s->draw();
		}
	};

	template<typename TShape>
	class Pack2 {
		std::vector<TShape>  _shapes;
		TShape* _ptr;
	public:
		Pack2()
			:_ptr(nullptr)
		{
			_shapes.reserve(100);
		}

		void add(TShape s) {
			_shapes.push_back(s);
		}

		size_t size() const {
			return _shapes.size();
		}

		TShape& at(size_t i) {
			if (i < _shapes.size())
				return _shapes.at(i);
		}

		TShape at(size_t i) const {
			if (i < _shapes.size())
				return _shapes.at(i);
		}

		void manage_collision(float r = 2.0f) {
			for (auto& s1 : _shapes)
				for (auto& s2 : _shapes)
				{
					if (physic::is_collid(s1, s2)) {

						physic::collision_process(s1, s2, r);
						break;
					}

				}
		}

		TShape* select_item(const fVec2 mouse) {
			_ptr = nullptr;
			for (auto& s : _shapes) {
				if (s.get_bounds().contain(mouse))
				{
					_ptr = &s;
					break;
				}
			}

			return _ptr;
		}

		void draw() const {
			for (const auto& s : _shapes) s.draw();
		}
	};

	template<size_t Capacity, typename ...Tshapes>
	class Pack3 {
		using Shapes = std::variant<Tshapes...>;
		
		std::vector<Shapes>  _shapes;
		Shapes*				 _ptr;

	public:
		Pack3()
			:_ptr(nullptr)
		{
			_shapes.reserve(Capacity);
		}

		void add(const Shapes& s) {
			_shapes.push_back(s);
		}

		size_t size() const {
			return _shapes.size();
		}

		Shapes& at(size_t i) {
			if (i < _shapes.size())
				return _shapes.at(i);
		}

		Shapes at(size_t i) const {
			if (i < _shapes.size())
				return _shapes.at(i);
		}

		bool IsCollid(const Shapes& s1, const Shapes& s2) {
			return std::visit([](const auto& a, const auto& b)->bool {
				return physic::is_collid(a, b);
				}, s1, s2);
		}

		void manage_collision(float r = 2.0f) { //<decltype(s1), decltype(s2)>

			for (auto& s1 : _shapes)
				for (auto& s2 : _shapes)
				{
					if (IsCollid(s1, s2))
					{
						std::visit([r](auto& s1, auto& s2)->void {
							physic::collision_process(s1, s2, r);
							}, s1, s2);
					}
				}
		}

		//TShape* select_item(const fVec2 mouse) {
		//	_ptr = nullptr;
		//	for (auto& s : _shapes) {
		//		if (s.get_bounds().contain(mouse))
		//		{
		//			_ptr = &s;
		//			break;
		//		}
		//	}

		//	return _ptr;
		//}

		void draw() const {
			for (auto& s : _shapes) 
				std::visit([](const auto& s) {
				s.draw();
					}, s);
		}
	};









	class Sstring : public console::IDrawable {
		std::wstring	_image;
		int				_colorbg;
		int				_colorfg;

		fVec2			_position;

	public:
		Sstring(std::wstring_view image, int colorfg, int colorbg)
			: _image(image)
			, _colorfg(colorfg)
			, _colorbg(colorbg)
		{}

		void set_position(const fVec2& p) {
			_position = p;
		}

		fVec2 get_position() const {
			return _position;
		}

		void draw() const {
			int i{};
			for (const auto& c : _image)
				konsole->draw(_position.x + i++, _position.y, c, _colorfg | (_colorbg << 4));
		}
	};

	template<typename TSprite>
	class Animation : public console::IDrawable {

		std::vector<TSprite>				vImages;

	public:

		Animation(std::initializer_list<TSprite> list)
		{
			for (auto it = list.begin(); it != list.end(); ++it)
			{
				it->set_position();
				vImages.emplace_back(*it);
			}
		}

		Animation(const std::vector<TSprite>& list_images)
			:vImages(list_images)
		{

		}

		void set_position() {

		}

		// TSprite should have method draw()
		void draw() const {
			for (const auto& image : vImages)
				image.draw();
		}
	};



       

        

   
}  // end namespace cgu