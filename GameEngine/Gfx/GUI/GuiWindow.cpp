#include "GuiWindow.hpp"
#include "../Texture.hpp"
#include "GuiWindowEvents.hpp"

#include <boost/bind.hpp>
#include <boost/make_shared.hpp>
#include <algorithm>

using namespace Spiral;
using namespace Spiral::GUI;

boost::int32_t GuiWindow::window_ID = 0;

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
m_windowId( window_ID++ ),
m_eventHandlers(),
m_hasFocus(false),
m_alpha(bAlpha),
m_dirty(false),
m_show(false),
m_allowFocus(true),
m_clipChildren(false),
m_parent(NULL)
{
}

GuiWindow::GuiWindow( const Math::SpVector2r& position, const Rect< SpReal >& rect, const boost::shared_ptr< Texture >& texture, bool bAlpha ):
m_localPosition( position ),
m_worldPosition( position ),
m_rect( rect ),
m_textCoords( Rect< SpReal >( 0.0f, 1.0f, 1.0f, 0.0f ) ),
m_texture( texture ),
m_children(),
m_windowId( window_ID++ ),
m_hasFocus(false),
m_alpha(bAlpha),
m_dirty(false),
m_show(false),
m_allowFocus(true),
m_clipChildren(false),
m_parent(NULL)
{
}

void GuiWindow::AddChild( const boost::shared_ptr< GuiWindow >& window )
{
	if( window )
	{
		m_children.push_back( window );
		window->SetParent( this );
		window->UpdatePositions( m_worldPosition );
	}

}

void GuiWindow::RemoveChild( const boost::shared_ptr< GuiWindow >& window )
{
	m_children.remove( window );
	window->SetParent( NULL );
}

void GuiWindow::RemoveChild( boost::uint32_t window_id )
{
	m_children.remove_if( boost::bind( &GuiWindow::GetID, _1 ) == window_id );
}

bool GuiWindow::SetFocus( SpReal x, SpReal y )
{
	bool hasFocus = false;
	bool inBounds = ContainsPoint( x, y );

	if( inBounds )
	{
		for( WindowItr itr = m_children.begin(); itr != m_children.end(); ++itr )
		{
			if( (*itr)->SetFocus( x, y ) )
			{
				hasFocus = true;
				break;
			}
		}

		if( !hasFocus && m_allowFocus )
		{
			if( m_hasFocus == false )
			{
				CallHandler( focus_gained, this, boost::any() );
			}
			// children refuse focus ( point does not lie within there bounds )
			m_hasFocus = true;

			if( lastFocusWindow && lastFocusWindow != this )
			{
				lastFocusWindow->CallHandler( focus_lost, lastFocusWindow, boost::any() );
				lastFocusWindow->m_hasFocus = false;
			}

			lastFocusWindow = this;

			// return that this window has recieved focus
			return m_hasFocus;
		}

	}else
	{
		// Replace this with some thing faster, maybe storing pointer to last focus window
		// like hover does
// 		for( WindowItr itr = m_children.begin(); itr != m_children.end(); ++itr )
// 		{
// 			(*itr)->SetFocus( x, y );
// 		}

		ResetWindow();
	}

// 	if( !inBounds && m_hasFocus == true ||
// 		inBounds && hasFocus )
// 	{		
// 		CallHandler( focus_lost, this, boost::any() );
// 		m_hasFocus = false;
// 	}
	
	// return that a child window has recieved focus
	return hasFocus;
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

void GuiWindow::MouseDown( const mouse_position& pos )
{
	if( m_show /*&& ContainsPoint( pos.x, pos.y )*/ )
	{
		ProcessMouseEvent( GUI::mouse_down, pos );
	}
}

void GuiWindow::MouseUp( const mouse_position& pos )
{
	if( m_show /*&& ContainsPoint( pos.x, pos.y )*/ )
	{
		ProcessMouseEvent( GUI::mouse_up, pos );
	}
}

void GuiWindow::CallHandler( boost::int32_t eventId, GuiWindow* window, const boost::any& data )
{
	std::pair< handleItr, handleItr > range_pair;
	range_pair = m_eventHandlers.equal_range( eventId );

	for( handleItr itr = range_pair.first; itr != range_pair.second; ++itr )
	{
		(*itr).second( eventId, window, data );
	}
}

void GuiWindow::ProcessEvent( boost::int32_t eventId, const boost::any& data )
{
	if( !m_hasFocus )
	{
		for( WindowItr itr = m_children.begin(); itr != m_children.end(); ++itr )
		{
			
			if( (*itr)->m_hasFocus )
			{
				(*itr)->CallHandler( eventId, (*itr).get(), data );
				break;
			}
			(*itr)->ProcessEvent( eventId, data );
		}
	}else
	{
		// call this windows event handler
		CallHandler( eventId, this, data );
	}
}

void GuiWindow::ProcessMouseEvent( boost::int32_t eventId, const mouse_position& pos )
{
	boost::any data( pos );
	SetFocus( pos.x, pos.y );

	// set focus and route calls to event functions of the window with focus
	ProcessEvent( eventId, data );
}

void GuiWindow::ResetWindow()
{
}

GuiWindow* GuiWindow::lastHoverWindow = NULL;
GuiWindow* GuiWindow::lastFocusWindow = NULL;
void GuiWindow::MouseHover( const mouse_position& pos )
{
	
	if( ContainsPoint( pos.x, pos.y ) )
	{
		GuiWindow* topMostWindow = FindTopMostChild( pos.x, pos.y );
		topMostWindow->CallHandler( GUI::mouse_hover, this, boost::any( pos ) );
		
		if( lastHoverWindow != NULL && lastHoverWindow != topMostWindow )
		{
			lastHoverWindow->ResetWindow();
			lastHoverWindow = NULL;
		}

		lastHoverWindow = topMostWindow;
 	}
	else
	{
		ResetWindow();	
	}
}

void GuiWindow::MouseMove( const mouse_position& pos )
{
	if( m_show )
	{
		ProcessEvent( GUI::mouse_move, boost::any( pos ) );
	}
}

void GuiWindow::CharInput( boost::uint32_t char_ )
{
	if( m_show )
	{
		ProcessEvent( GUI::char_input, boost::any( char_ ) );
	}
}


GuiWindow* GuiWindow::FindTopMostChild( SpReal x, SpReal y ) const
{
	GuiWindow* window = NULL;
	if( ContainsPoint( x, y ) && m_allowFocus )
	{
		for( Const_WindowItr itr = m_children.begin(); itr != m_children.end(); ++itr )
		{
			window = (*itr)->FindTopMostChild( x, y );
			if( window ) return window;
		}

		window = const_cast<GuiWindow*>(this);
	}

	return window;
}

GuiWindow* GuiWindow::GetChild( boost::uint32_t window_id ) const
{
	const_window_itr itr = std::find_if( m_children.begin(), m_children.end(), boost::bind( &GuiWindow::GetID, _1 ) == window_id );

	if( itr != m_children.end() )
	{
		return (*itr).get();
	}

	return NULL;
}

