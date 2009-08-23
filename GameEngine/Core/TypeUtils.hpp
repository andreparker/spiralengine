/*!
*/
#ifndef TYPE_UTILS_HPP
#define TYPE_UTILS_HPP

#include <boost/cstdint.hpp>
#include <functional>

namespace Spiral
{

	template< boost::int32_t Bit >
	struct Int2Bit
	{
		static const boost::int32_t value = Bit << 1;
	};

	template< class T, T compVal >
	struct equal_to_val : public std::binary_function< T, T, bool >
	{
		bool operator()( const T& val )
		{
			return bool(val == compVal);
		}
	};

	template< class Itr, class Cond, class Func >
	void foreach_if( Itr first, Itr last,Func func, Cond cond )
	{
		while( first++ != last )
		{
			if( cond( *first ) )
			{
				func( *first );
			}
		}
	}

	template< class Itr >
	boost::int32_t count_elements( Itr first, Itr last )
	{
		boost::int32_t count = 0;
		while( first++ != last ) ++count;

		return count;
	}

	template< class T >
	struct EmptyType
	{
		typedef T type;
	};
}

#endif