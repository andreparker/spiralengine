/*!
*/
#ifndef EVENT_DATA_HPP
#define EVENT_DATA_HPP

#include "Event.hpp"
#include <boost/any.hpp>

namespace Spiral
{

namespace detail
{
	struct EventData
	{
		Event event;
		boost::any data;
	};
}

typedef detail::EventData EventData;
}
#endif