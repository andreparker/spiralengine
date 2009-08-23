/*!
*/
#ifndef STATE_EVENT_IMPL_HPP
#define STATE_EVENT_IMPL_HPP

#include <boost/cstdint.hpp>
#include <boost/any.hpp>

namespace Spiral
{
namespace Impl
{
	struct StateEvent
	{
		boost::int32_t eventType;
		boost::any data;
	};
}

typedef Impl::StateEvent StateEvent;

}
#endif