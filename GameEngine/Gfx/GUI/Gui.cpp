#include "Gui.hpp"
#include "../GfxDriver.hpp"
#include "../RenderState.hpp"
#include "../../Core/MouseEvent.hpp"
#include "../../Core/Events.hpp"
#include "GuiWindow.hpp"
#include "GuiWindowEvents.hpp"
#include <algorithm>
#include <boost/bind.hpp>

using namespace Spiral;
using namespace Spiral::GUI;

GuiManager::GuiManager():
m_windowList()
{}

void GuiManager::AddElement( const boost::shared_ptr< GuiWindow >& window )
{
	m_windowList.push_back( window );
}

void GuiManager::RemoveElement( const boost::shared_ptr< GuiWindow >& window )
{
	m_windowList.remove( window );
}

void GuiManager::TraverseRender( const boost::shared_ptr< GfxDriver >& gfxDrvier, const boost::shared_ptr< GuiWindow >& window )const
{
	if( window->IsVisible() )
	{
		if( window->GetTexture() )
		{
			gfxDrvier->Bind( window->GetTexture() );
			if( window->hasAlpha() )
			{
				gfxDrvier->SetState( RenderState::Blend( RenderState::RS_TRUE ) );
				gfxDrvier->Draw( window->GetWorldPosition(), window->GetRect(), window->GetTextCoords() );
				gfxDrvier->SetState( RenderState::Blend( RenderState::RS_FALSE ) );
			}else
			{
				gfxDrvier->Draw( window->GetWorldPosition(), window->GetRect(), window->GetTextCoords() );
			}
			
		}

		for( const_gui_window_itr itr = window->m_children.begin(); itr != window->m_children.end(); ++itr )
		{
			TraverseRender( gfxDrvier, *itr );
		}
	}
}

void GuiManager::Input( const Event& inputEvent, const boost::any& data )
{
	if( inputEvent.hasCat( Catagory_Mouse::value ) )
	{
		MouseEvent event_data = boost::any_cast< MouseEvent >( data );
		mouse_position position( static_cast<SpReal>(event_data.pos.x), static_cast<SpReal>(event_data.pos.y) );
		HandleMouseInput( inputEvent, position );
	}
}

void GuiManager::HandleMouseInput( const Event& inputEvent, const mouse_position& position )
{
	if( inputEvent.IsCat( Catagory_Mouse_MouseDown::value ) )
	{
		std::for_each( m_windowList.begin(), m_windowList.end(), boost::bind( &GuiWindow::MouseDown, _1, boost::cref(position) ) );
	}
	if( inputEvent.IsCat( Catagory_Mouse_Up::value ) )
	{
		std::for_each( m_windowList.begin(), m_windowList.end(), boost::bind( &GuiWindow::MouseUp, _1, boost::cref(position) ) );
	}
	if( inputEvent.IsCat( Catagory_Mouse_Move::value ) )
	{
		std::for_each( m_windowList.begin(), m_windowList.end(), boost::bind( &GuiWindow::MouseHover, _1, boost::cref(position) ) );
	}
}

void GuiManager::HandleCharInput( const Event& inputEvent, boost::uint32_t char_ )
{
	std::for_each( m_windowList.begin(), m_windowList.end(), boost::bind( &GuiWindow::CharInput, _1, char_ ) ) ;
}

void GuiManager::Present( const boost::shared_ptr< GfxDriver >& gfxDrvier )
{
	std::for_each( m_windowList.begin(), m_windowList.end(), boost::bind( &GuiManager::TraverseRender, this, boost::cref( gfxDrvier ), _1 ) );
}


