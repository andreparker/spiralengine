#include "UpdateQueue.hpp"

using namespace Spiral;
using namespace boost;

void UpdateQueue::Add( const UPDATE_CALL_BACK& callback, SpReal delayTick/* = 0.0f */ )
{
	CallBackInfo info( callback, delayTick, m_ticks );
	m_callbacks.push_back( info );
}

void UpdateQueue::Tick( SpReal tick )
{
	std::list< CallBackInfo >::iterator itr = m_callbacks.begin();
	std::list< CallBackInfo >::iterator itrLast;
	m_ticks += tick;

	while( itr != m_callbacks.end() )
	{
		CallBackInfo& info = *itr;
		if( m_ticks - info.m_currentStartTick > info.m_delayTick )
		{
			info.callback();
			itrLast = itr++;
			m_callbacks.erase( itrLast );
			continue;
		}

		++itr;
	}
}

UpdateQueue::UpdateQueue():
m_callbacks(),m_ticks(0.0f)
{

}