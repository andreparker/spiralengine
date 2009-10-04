#include "GuiWindow.hpp"
#include "../Texture.hpp"
#include <boost/bind.hpp>
#include <boost/make_shared.hpp>
#include <algorithm>

using namespace Spiral;
using namespace Spiral::GUI;

void GuiWindow::UpdatePositions( const Math::SpVector2r& worldPosition )
{
	m_worldPosition = worldPosition + m_localPosition;
	std::for_each( m_children.begin(), m_children.end(), boost::bind( &GuiWindow::UpdatePositions, _1, boost::cref(m_worldPosition) ) );
}

GuiWindow::GuiWindow( const Math::SpVector2r& position, const Rect< SpReal >& rect, const Rect< SpReal >& textCoords,const boost::shared_ptr< Texture >& texture, bool bAlpha ):
m_localPosition( position ),
m_worldPosition( position ),
m_rect( rect ),
m_textCoords( textCoords ),
m_texture( texture ),
m_children(),
m_windowId(0),
m_eventHandlers(),
m_hasFocus(false),
m_alpha(bAlpha),
m_dirty(true)
{
}

GuiWindow::GuiWindow( const Math::SpVector2r& position, const Rect< SpReal >& rect, const boost::shared_ptr< Texture >& texture, bool bAlpha ):
m_localPosition( position ),
m_worldPosition( position ),
m_rect( rect ),
m_textCoords( Rect< SpReal >( 0.0f, 1.0f, 1.0f, 0.0f ) ),
m_texture( texture ),
m_children(),
m_windowId(0),
m_hasFocus(false),
m_alpha(bAlpha),
m_dirty(true),
m_show(false)
{
}

void GuiWindow::AddChild( const boost::shared_ptr< GuiWindow >& window )
{
	m_children.push_back( window );
}

void GuiWindow::RemoveChild( const boost::shared_ptr< GuiWindow >& window )
{
	m_children.remove( window );
}

void GuiWindow::RemoveChild( boost::uint32_t window_id )
{
	m_children.remove_if( boost::bind( &GuiWindow::GetID, _1 ) == window_id );
}

bool GuiWindow::SetFocus( SpReal x, SpReal y )
{
	bool hasFocus = m_hasFocus = false;

	// translate into local space
	SpReal localx = x - m_worldPosition[0];
	SpReal localy = y - m_worldPosition[1];

	if( m_rect.Contains( x, y ) )
	{
		for( WindowItr itr = m_children.begin(); itr != m_children.end(); ++itr )
		{
			if( (*itr)->SetFocus( x, y ) )
			{
				hasFocus = true;
			}
		}

		if( !hasFocus )
		{
			// children refuse focus ( point does not lie within there bounds )
			m_hasFocus = true;
		}
	}
	
	return m_hasFocus;
}

boost::shared_ptr< GuiWindow > GuiWindow::Create( const Math::SpVector2r& position, const Rect< SpReal >& rect, const boost::shared_ptr< Texture >& texture, bool bAlpha )
{
	return boost::make_shared< GuiWindow >( position, rect, texture, bAlpha );
}

boost::shared_ptr< GuiWindow > GuiWindow::Create( const Math::SpVector2r& position, const Rect< SpReal >& rect, const Rect< SpReal >& textCoords, boost::shared_ptr< Texture >& texture, bool bAlpha )
{
	return boost::make_shared< GuiWindow >( position, rect, textCoords, texture, bAlpha );
}

void GuiWindow::ConnectHandler( boost::int32_t eventId, const WindowEventHandler& handler )
{
	m_eventHandlers.insert( std::pair< boost::int32_t, WindowEventHandler >( eventId, handler ) );
}

void GuiWindow::DisConnectHandler( boost::int32_t eventId )
{
	m_eventHandlers.erase( eventId );
}