/*!
*/
#ifndef EVENT_HPP
#define EVENT_HPP

#include <boost/cstdint.hpp>
#include "EventCatagory.hpp"

namespace Spiral
{
	struct Event
	{
		enum
		{
			EVENT_ANY = 0xffffffff
		};

		Event( boost::int32_t event, const EventCatagory& catagory ):
		m_eventId( event ), m_catagory( catagory ){}

		Event( boost::int32_t event, boost::int32_t catagoryBits ):
		m_eventId( event ), m_catagory( catagoryBits ){}

		Event():m_eventId(EVENT_ANY),m_catagory(){}

		bool operator==( const Event& event )const
		{
			bool result1 = true;
			if( m_eventId != EVENT_ANY )
			{
				result1 = bool( m_eventId == event.m_eventId );
			}

			std::bitset<32> result = m_catagory.m_bits;
			result &= event.m_catagory.m_bits; // and the catagorys together
			return ( m_catagory.m_bits == result ) && result1; // result must be the same after the and
			
		}

		boost::int32_t m_eventId;
		EventCatagory m_catagory;
	};
}

#endif