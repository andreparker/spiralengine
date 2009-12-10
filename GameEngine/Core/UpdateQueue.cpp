#include "UpdateQueue.hpp"

using namespace Spiral;
using namespace boost;

void UpdateQueue::Add( const UPDATE_CALL_BACK& callback )
{
	m_callbacks.push( callback );
}

void UpdateQueue::Update()
{
	while( !m_callbacks.empty() )
	{
		UPDATE_CALL_BACK callback = m_callbacks.top();
		callback();
		m_callbacks.pop();
	}
}