#include "EventPublisher.hpp"
#include "EventData.hpp"
#include "EventSubscriber.hpp"

#include <boost/thread.hpp>
#include <boost/cstdint.hpp>
#include <boost/bind.hpp>
#include <algorithm>

using namespace Spiral;
using namespace boost;

const int32_t maxQueueSize = 15;

EventPublisher::EventPublisher():
m_subscribers(),
m_eventQueue(),
m_mutex()
{
	m_eventQueue.reset( new std::queue< detail::EventData > );
}

void EventPublisher::RemoveSubscriber( boost::shared_ptr<EventSubscriber>& subscriber )
{
	while( m_mutex.try_lock() == false ){}
	m_subscribers.remove( subscriber );
	m_mutex.unlock();
}

void EventPublisher::AddSubscriber( boost::shared_ptr<EventSubscriber>& subscriber )
{
	while( m_mutex.try_lock() == false ){}
	m_subscribers.push_back( subscriber );
	m_mutex.unlock();
}

void EventPublisher::Publish( const Event& event, boost::any& data )
{
	detail::EventData eventData;

	eventData.event = event;
	eventData.data  = data;
	
	while( m_mutex.try_lock() == false ){}

	if( maxQueueSize > m_eventQueue->size() )
	{
		m_eventQueue->push( eventData );
	}
	
	m_mutex.unlock();
}

void EventPublisher::ProcessEventQueue()
{
	while( true )
	{
		if( m_mutex.try_lock() )
		{
			while( !m_eventQueue->empty() )
			{
				detail::EventData eventData = m_eventQueue->front();
				m_eventQueue->pop();

				for( SubIter itr = m_subscribers.begin(); itr != m_subscribers.end(); ++itr )
				{
					EventSubscriber* subscriber = (*itr).get();
					if( subscriber->GetEvent() == eventData.event )
					{
						subscriber->Recieve( eventData );
					}
				}
			}
			m_mutex.unlock();
		}
		this_thread::yield();
	}
	
}