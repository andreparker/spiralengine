/*!
*/
#ifndef EVENT_PUBLISHER_HPP
#define EVENT_PUBLISHER_HPP

#include <boost/any.hpp>
#include <boost/noncopyable.hpp>
#include <list>

#include <boost/shared_ptr.hpp>
#include "EventDataFwd.hpp"

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
		void Publish( const Event& event, boost::any& data );

	private:
		typedef std::list< boost::shared_ptr<EventSubscriber> >::iterator SubIter;
		std::list< boost::shared_ptr<EventSubscriber> > m_subscribers;
	};
}

#endif