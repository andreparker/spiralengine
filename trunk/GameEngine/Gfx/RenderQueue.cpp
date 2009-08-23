#include <algorithm>
#include <boost/foreach.hpp>
#include <boost/bind.hpp>

#include "GfxDriver.hpp"
#include "RenderEntry.hpp"
#include "RenderQueue.hpp"
#include "RenderQueueException.hpp"

using namespace Spiral;
using namespace boost;
using namespace std;

RenderQueue::RenderQueue():
m_entryList()
{
}

void RenderQueue::AddEntry( RenderEntry* entry )
{
	if( entry == NULL )
	{
		throw RenderQueueException( "Invalid render entry!" );
	}

	m_entryList.push_front( entry );
}

void RenderQueue::Sort( bool bOrder /*= true*/ )
{
	if( bOrder == true )
	{
		//m_entryList.sort();
	}else
	{
		// reverse order
		//m_entryList.sort( std::greater< RenderEntry >() );
	}
}

void RenderQueue::Present( boost::shared_ptr< GfxDriver >& gfxDriver )
{
	for_each( m_entryList.begin(), m_entryList.end(), bind( &RenderEntry::Render, _1, gfxDriver ) );
}

void RenderQueue::Clear()
{
	m_entryList.clear();
}

bool RenderQueue::GetEntry( RenderEntry*& entry, boost::int32_t id )
{

	EntryItr itr = find_if( m_entryList.begin(), m_entryList.end(), ( bind( &RenderEntry::GetId, _1 ) == id ) );

	entry = &(*itr);
	
	return bool( itr != m_entryList.end() );
}