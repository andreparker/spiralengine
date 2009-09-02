/*!
*/
#ifndef EVENT_SUBSCRIBER_HPP
#define EVENT_SUBSCRIBER_HPP

#include "Event.hpp"
#include "EventDataFwd.hpp"

#include <list>
#include <boost/function.hpp>
#include <boost/any.hpp>
#include <boost/noncopyable.hpp>

namespace Spiral
{

	
	typedef boost::function< void( const Event&, const boost::any& ) > EventCallbackFunc;
	class EventSubscriber : boost::noncopyable
	{
	public:
		EventSubscriber( const Event& event );

		/*!
		   @function  AddCallback
		   @brief     adds a call back to be called when a subscribed event is recieved
		   @return    void
		   @param     EventCallbackFunc & func
		*/
		void AddCallback( EventCallbackFunc& func );

		/*!
		   @function  Recieve
		   @brief     private method only callable by EventPublisher
		   @return    void
		   @param     const EventData & data
		*/
		void Recieve( const EventData& data )const;

		/*!
		   @function  GetEvent
		   @brief     returns the subscribed event
		   @return    const Event&
		*/
		const Event& GetEvent()const
		{
			return m_event;
		}

	private:
		EventSubscriber();
		const Event m_event;
		std::list< EventCallbackFunc > m_callBacks;
	};
}

#endif