#ifndef MOUSE_EVENT_HPP
#define MOUSE_EVENT_HPP

#include <boost/cstdint.hpp>

namespace Spiral
{
	struct MouseEvent
	{
		

		typedef enum
		{
			mouse1,
			mouse2,
			mouse3,
			mouse4,
			mouse5,
			max_mouse_input
		}MouseIdType;

		MouseEvent( MouseIdType id_, boost::int32_t x, boost::int32_t y ):
			pos( x, y ), mouseid( id_ ){}
		
		MouseIdType mouseid;

		struct mouse_pos
		{
			mouse_pos( boost::int32_t x_, boost::int32_t y_ ):
			x(x_),y(y_){}
			
			boost::uint32_t x,y;
		}pos;
	};
}
#endif