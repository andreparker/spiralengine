#ifndef EVENT_HPP
#define EVENT_HPP

#include "object.hpp"
#include "linkedlist.ipp"
#include "Templates/FastDelegate.h"

namespace gameEngine
{
	struct EventData;

	class Event
	{
		public:
			typedef fastdelegate::FastDelegate2< object*, const EventData* > callBackFunc;

			struct eventUser
			{
				void * userId;
				callBackFunc func;
				bool operator!=( const eventUser& e )const
				{
					return ( userId != e.userId ) && ( func != func );
				}

				bool operator==( const eventUser& e )const
				{
					return ( userId == e.userId ) && ( func == func );
				}

			};

			template< class C >
			class find_user_func
			{
				public:
					find_user_func( void* obj, void (C::*func)( object*, const EventData* ) );
					bool operator()( const eventUser& user );
				protected:
					eventUser test;
			};

			template< class T, class C >
			void registerListener( T* objectBind, void (C::*func)( object*, const EventData* ) );

			template< class T, class C >
			bool unRegisterListener( T* objectBind, void (C::*func)( object*, const EventData* ) );

			virtual void passEvent( object* obj, const EventData* eventdata );
		protected:
			typedef common::linkedList< eventUser >::iterator callBackItr;
			common::linkedList< eventUser > m_callbacks;
	};

	struct EventData
	{
		int32 id;
	};

	template< class C >
	Event::find_user_func<C>::find_user_func( void* obj, void (C::*func)( object*, const EventData* ) )
	{
		test.func = func;
		test.userId = obj;
	}

	template< class C >
	bool Event::find_user_func<C>::operator ()(const Event::eventUser &user)
	{
		return bool( test == user );
	}

	template< class T, class C >
	void Event::registerListener( T* objectBind, void (C::*func)( object*, const EventData* ) )
	{
		eventUser user;
		callBackFunc newDelegate = fastdelegate::MakeDelegate( objectBind, &func );

		user.func = newDelegate;
		user.userId = reinterpret_cast< void* >( objectBind );

		m_callbacks.insertTail( user );
	}

	template< class T, class C >
	bool Event::unRegisterListener( T* objectBind, void (C::*func)( object*, const EventData* ) )
	{
		bool isFound = false;
		callBackItr found = common::find_if( m_callbacks.head(), 
			m_callbacks.end(), Event::find_user_func<C>( reinterpret_cast<void*>(objectBind), func ) );

		if( found != m_callbacks.end() )
		{
			// remove from list
			m_callbacks.remove( found );
			isFound = true;
		}

		return isFound;
	}

}

#endif