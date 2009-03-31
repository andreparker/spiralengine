#include "event.hpp"

using namespace gameEngine;

void Event::passEvent( object* obj, const EventData* eventdata )
{
	callBackItr itr = m_callbacks.head();
	for(; itr != m_callbacks.end(); ++itr )
	{
		// call the callback
		eventUser &e = *itr;
		e.func( obj, eventdata );
	}
}
