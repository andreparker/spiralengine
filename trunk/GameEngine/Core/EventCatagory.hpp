/*!
*/
#ifndef EVENT_CATAGORY_HPP
#define EVENT_CATAGORY_HPP

#include <boost/cstdint.hpp>
#include <bitset>

namespace Spiral
{

	struct EventCatagory
	{
		EventCatagory( boost::int32_t bits ):
		m_bits( bits ){}
		EventCatagory():m_bits(){}
		
		std::bitset<32> m_bits;
	};
}

#endif