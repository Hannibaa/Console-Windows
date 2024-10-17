/*				ABSTRACT CLASS OF SNAKE							
				1. OBJECT COLLECTION
				2. MOTION COLLECTION
				3. BEHAVIOR COLLECTION
*/
#pragma once
#include <console project/basic/ka_IDrawable.hpp>
#include <console project/basic/ka_ITransformable.hpp>
#include <console project/console/ka_utility.hpp>

namespace snake {
	// define mane drawable object here
	class TDrawable;

	// make function generator of this type
	template<typename ...Ts>
	TDrawable  make_type(Ts...args);

//////////////////////////////////////////////////////////////////////////////////////////////////////////
// 
//		Interface for snake    
// 
//////////////////////////////////////////////////////////////////////////////////////////////////////////
	

	
	class ISnake : public console::IDrawable, public console::ITransformable<float>
	{
	public:
		using value_type = TDrawable;
		using number_type = typename console::ITransformable<float>::value_type;
		using vector_type = typename console::ITransformable<float>::vector_type;
		using container_type = std::vector<value_type>;

		//value_type								_X0;
	private:

		container_type							_X;
		size_t									_size;

	public:
		ISnake(const vector_type& p0) {
			auto _X0 = make_type();
			_X.push_back(_X0);
			_X[0].set_position(p0);
			_size = _X.size();
		}


	private:
		void init(const vector_type& p0);
	};



}