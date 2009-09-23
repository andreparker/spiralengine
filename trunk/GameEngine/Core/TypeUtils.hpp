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

	template< boost::uint32_t byte0, boost::uint32_t byte1, boost::uint32_t byte2, boost::uint32_t byte3 >
	struct MakeLongValue
	{
		static const boost::uint32_t value = (byte0 & 0xff) | ((byte1 & 0xff) << 8) | ((byte2 & 0xff) << 16) | ((byte3 & 0xff) << 24);
	};

	template< boost::uint32_t byteNumber >
	inline boost::uint32_t GetByteFromLong( boost::uint32_t value )
	{
		const boost::uint32_t mask = 0xff << (byteNumber * 8);
		boost::uint32_t retValue = mask & value;
		return retValue >> (byteNumber * 8);
	}

	template< boost::uint32_t bits, int place = 0 >
	struct Binary2Dec
	{
		enum
		{
			holePart = bits / 10L
		};
		static const boost::uint32_t value = ( static_cast<boost::uint32_t>(  ( float( bits * 0.1f - holePart ) > 0.0f ? 1 : 0 )  ) << place ) | 
			Binary2Dec< holePart, place+1 >::value;
	};

	template< int place >
	struct Binary2Dec< 0, place >
	{
		static const boost::uint32_t value = 0;
	};

	template< boost::int32_t Bit >
	struct Int2Bit
	{
		static const boost::int32_t value = 1 << Bit;
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

	template< class Itr, class elem >
	boost::int32_t count_element_occurences( Itr first, Itr last, elem e )
	{
		boost::int32_t count = 0;
		while( first != last )
		{
			if( *first == e )
				++count;

			++first;
		}

		return count;
	}

	template< class T >
	struct EmptyType
	{
		typedef T type;
	};
}

#endif