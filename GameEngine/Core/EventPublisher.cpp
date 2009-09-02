#include "EventPublisher.hpp"
#include "EventData.hpp"
#include "EventSubscriber.hpp"


#include <boost/bind.hpp>
#include <algorithm>

using namespace Spiral;
using namespace boost;

EventPublisher::EventPublisher():
m_subscribers()
{
}

void EventPublisher::RemoveSubscriber( boost::shared_ptr<EventSubscriber>& subscriber )
{
	m_subscribers.remove( subscriber );
}

void EventPublisher::AddSubscriber( boost::shared_ptr<EventSubscriber>& subscriber )
{
	m_subscribers.push_back( subscriber );
}

void EventPublisher::Publish( const Event& event, boost::any& data )
{
	Spiral::detail::EventData eventData;

	eventData.event = event;
	eventData.data  = data;

	for( SubIter itr = m_subscribers.begin(); itr != m_subscribers.end(); ++itr )
	{
		EventSubscriber* subscriber = (*itr).get();
		if( subscriber->GetEvent() == event )
		{
			subscriber->Recieve( eventData );
		}
	}

}