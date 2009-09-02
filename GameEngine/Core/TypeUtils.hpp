/*!
*/
#ifndef TYPE_UTILS_HPP
#define TYPE_UTILS_HPP

#include <boost/cstdint.hpp>
#include <functional>

#define BIT_ON1( b0 ) Int2Bit< b0 >::value
#define BIT_ON2( b0, b1 ) ValueChain< Int2Bit< b0 >::value, Int2Bit< b1 >::value >::value
#define BIT_ON3( b0, b1, b2 ) ValueChain< ValueChain< Int2Bit< b0 >::value, Int2Bit< b1 >::value >::value, Int2Bit< b2 >::value >::value
#define BIT_ON4( b0, b1, b2, b3 ) ValueChain< ValueChain< ValueChain< Int2Bit< b0 >::value, Int2Bit< b1 >::value >::value, Int2Bit< b2 >::value >::value, Int2Bit< b3 >::value >::value
#define BIT_ON5( b0, b1, b2, b3, b4 ) ValueChain< ValueChain< ValueChain< ValueChain< Int2Bit< b0 >::value, Int2Bit< b1 >::value >::value, Int2Bit< b2 >::value >::value, Int2Bit< b3 >::value >::value, Int2Bit< b4 >::value >::value

namespace Spiral
{

	template< boost::int32_t Bit >
	struct Int2Bit
	{
		static const boost::int32_t value = Bit << 1;
	};

	template< boost::int32_t val1, boost::int32_t val2 >
	struct ValueChain
	{
		static const boost::int32_t value = val1 | val2;
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