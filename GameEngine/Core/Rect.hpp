/*!
*/
#ifndef RECT_HPP
#define RECT_HPP

namespace Spiral
{
	template< class T >
	struct Rect
	{
		typedef T type;
		Rect(): left(0),right(0),bottom(0),top(0){}
		Rect( const T& left_, const T& right_, const T& bottom_, const T& top_ ):
			left(left_),right(right_),bottom(bottom_),top(top_){}

		Rect( const Rect& rect_ ):
			left( rect_.left ), right( rect_.right ), bottom( rect_.bottom ),top( rect_.top ){}

		bool Contains( const Rect& rect )const
		{
			bool isInBounds = false;
			if( left <= rect.left && right >= rect.right )
			{
				if( top <= rect.top && bottom >= rect.bottom )
				{
					isInBounds = true;
				}
			}

			return isInBounds;
		}


		T left, right, bottom, top;
	};
}

#endif