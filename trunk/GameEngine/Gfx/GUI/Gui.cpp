#include "Gui.hpp"
#include "../GfxDriver.hpp"
#include "GuiWindow.hpp"
#include <algorithm>
#include <boost/bind.hpp>

using namespace Spiral;
using namespace Spiral::GUI;

GuiManager::GuiManager():
m_windowList()
{}

void GuiManager::AddWindow( const boost::shared_ptr< GuiWindow >& window )
{
	m_windowList.push_back( window );
}

void GuiManager::RemoveWindow( const boost::shared_ptr< GuiWindow >& window )
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
			gfxDrvier->Draw( window->GetWorldPosition(), window->GetRect(), window->GetTextCoords() );
		}

		for( const_gui_window_itr itr = window->m_children.begin(); itr != window->m_children.end(); ++itr )
		{
			TraverseRender( gfxDrvier, *itr );
		}
	}
}

void GuiManager::MouseInput( const Event& mouseEvent, const boost::any& data )
{

}

void GuiManager::Present( const boost::shared_ptr< GfxDriver >& gfxDrvier )
{
	std::for_each( m_windowList.begin(), m_windowList.end(), boost::bind( &GuiManager::TraverseRender, this, boost::cref( gfxDrvier ), _1 ) );
}