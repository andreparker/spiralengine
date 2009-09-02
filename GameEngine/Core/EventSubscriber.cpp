#include "EventSubscriber.hpp"
#include "EventData.hpp"

using namespace Spiral;

void EventSubscriber::AddCallback( EventCallbackFunc& func )
{
	m_callBacks.push_back( func );
}

void EventSubscriber::Recieve( const detail::EventData& data ) const
{
	typedef std::list< EventCallbackFunc >::const_iterator const_itr;

	for( const_itr itr = m_callBacks.begin(); itr != m_callBacks.end(); ++itr )
	{
		(*itr)( data.event, data.data );
	}
}

EventSubscriber::EventSubscriber( const Event& event ):
m_event( event ),
m_callBacks()
{
}