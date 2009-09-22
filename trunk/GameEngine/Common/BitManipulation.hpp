#ifndef BIT_MANIPULATION_HPP
#define BIT_MANIPULATION_HPP

#include <boost/cstdint.hpp>

namespace Spiral
{

	inline bool PowerOf2( boost::uint32_t n )
	{
		return bool( n && ( (n-1) & n ) == 0 );
	}

	inline boost::uint32_t NextPowerOf2( boost::uint32_t n )
	{
		--n;

		n |= n >> 1;
		n |= n >> 2;
		n |= n >> 4;
		n |= n >> 8;
		n |= n >> 16;
		
		return (n+1);
	}

}
#endif