#include "EventPublisher.hpp"
#include "EventData.hpp"
#include "EventSubscriber.hpp"

#include <boost/thread.hpp>
#include <boost/cstdint.hpp>
#include <boost/bind.hpp>
#include <algorithm>

using namespace Spiral;
using namespace boost;

const int32_t maxQueueSize = 50;

EventPublisher::EventPublisher():
m_subscribers(),
m_eventQueue(),
m_eventQueueMutex(),
m_subscriberMutex()
{
	m_eventQueue.reset( new std::queue< detail::EventData > );
}

inline void Lock_Wait( boost::mutex& m )
{
	while( m.try_lock() == false ){}
}

void EventPublisher::RemoveSubscriber( const boost::shared_ptr<EventSubscriber>& subscriber )
{
	Lock_Wait( m_subscriberMutex );
	m_subscribers.remove( subscriber );
	m_subscriberMutex.unlock();
}

void EventPublisher::AddSubscriber( const boost::shared_ptr<EventSubscriber>& subscriber )
{
	Lock_Wait( m_subscriberMutex );
	m_subscribers.push_back( subscriber );
	m_subscriberMutex.unlock();
}

void EventPublisher::Publish( const Event& event, const boost::any& data )
{
	detail::EventData eventData;

	eventData.event = event;
	eventData.data  = data;
	
	Lock_Wait( m_eventQueueMutex );

	if( maxQueueSize > m_eventQueue->size() )
	{
		m_eventQueue->push( eventData );
	}
	
	m_eventQueueMutex.unlock();
}

void EventPublisher::ProcessEventQueueThreaded()
{
	while( true )
	{
		while( !m_eventQueue->empty() )
		{
			Lock_Wait( m_eventQueueMutex );

			detail::EventData eventData = m_eventQueue->front();
			m_eventQueue->pop();

			m_eventQueueMutex.unlock();

			Lock_Wait( m_subscriberMutex );
			for( SubIter itr = m_subscribers.begin(); itr != m_subscribers.end(); ++itr )
			{
				EventSubscriber* subscriber = (*itr).get();
				if( subscriber->GetEvent() == eventData.event )
				{
					subscriber->Recieve( eventData );
				}
			}
			m_subscriberMutex.unlock();
		}
		this_thread::yield();
	}

}

void EventPublisher::ProcessEventQueue()
{

	while( !m_eventQueue->empty() )
	{
		detail::EventData& eventData = m_eventQueue->front();
		

		for( SubIter itr = m_subscribers.begin(); itr != m_subscribers.end(); ++itr )
		{
			EventSubscriber* subscriber = (*itr).get();
			if( subscriber->GetEvent() == eventData.event )
			{
				subscriber->Recieve( eventData );
			}
		}
		
		m_eventQueue->pop();
	}

}