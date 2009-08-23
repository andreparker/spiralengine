#include "RenderEntry.hpp"

using namespace Spiral;

RenderEntry::RenderEntry( bool bhasAlpha, boost::int32_t id ):
m_bAlpha( bhasAlpha ),
m_id( id ),
m_zOrder( 1.0f )
{

}

RenderEntry::~RenderEntry()
{
}

bool RenderEntry::operator<(  const RenderEntry& entry1 )const
{
	return bool( GetZOrder() < entry1.GetZOrder() );
}

bool RenderEntry::operator>(  const RenderEntry& entry1 )const
{
	return bool( GetZOrder() > entry1.GetZOrder() );
}

void RenderEntry::Render( boost::shared_ptr< GfxDriver >& gfxDriver )
{
	DoRender( gfxDriver );
}

