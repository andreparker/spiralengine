/*!
*/
#ifndef EVENT_PUBLISHER_HPP
#define EVENT_PUBLISHER_HPP

#include <queue>
#include <boost/any.hpp>
#include <boost/noncopyable.hpp>
#include <boost/thread/mutex.hpp>
#include <list>

#include <boost/scoped_ptr.hpp>
#include <boost/shared_ptr.hpp>
#include "EventData.hpp"

namespace Spiral
{
	class EventSubscriber;
	class EventPublisher : boost::noncopyable
	{
	public:
		EventPublisher();

		/*!
		   @function  RemoveSubscriber
		   @brief     adds a subscriber of a event to the class
		   @return    void
		   @param     boost::shared_ptr<EventSubscriber> & subscriber
		*/
		void RemoveSubscriber( boost::shared_ptr<EventSubscriber>& subscriber );

		/*!
		   @function  AddSubscriber
		   @brief     removes a subscriber
		   @return    void
		   @param     boost::shared_ptr<EventSubscriber> & subscriber
		*/
		void AddSubscriber( boost::shared_ptr<EventSubscriber>& subscriber );

		/*!
		   @function  Publish
		   @brief     publishes a event to all subscribers of it
		   @return    void
		   @param     const Event & event
		   @param     boost::any & data
		*/
		void Publish( const Event& event, const boost::any& data );

		/*!
		   @function  ProcessEventQueueThreaded
		   @brief     process events stored away and send them to subscribers,
		              this has thread code in it
		   @return    void
		*/
		void ProcessEventQueueThreaded();

		/*!
		   @function  ProcessEventQueue
		   @brief     process events stored away and send them to subscribers
		   @return    void
		*/
		void ProcessEventQueue();
	private:
		typedef std::list< boost::shared_ptr<EventSubscriber> >::iterator SubIter;
		std::list< boost::shared_ptr<EventSubscriber> > m_subscribers;
		boost::scoped_ptr< std::queue< EventData > > m_eventQueue;
		boost::mutex m_eventQueueMutex;
		boost::mutex m_subscriberMutex;
	};
}

#endif