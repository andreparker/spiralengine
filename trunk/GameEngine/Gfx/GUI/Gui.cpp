#include "Gui.hpp"
#include "../../Core/Engine.hpp"
#include "../GfxDriver.hpp"
#include "../GfxImpl.hpp"
#include "../RenderState.hpp"
#include "../../Core/MouseEvent.hpp"
#include "../../Core/Events.hpp"
#include "../Texture.hpp"
#include "../Color.hpp"
#include "../Font.hpp"
#include "../../Math/Math.hpp"
#include "GuiWindow.hpp"
#include "GuiWindowEvents.hpp"
#include <algorithm>
#include <boost/bind.hpp>
#include <boost/make_shared.hpp>

#include "GuiButton.hpp"
#include "GuiSlider.hpp"
#include "GuiText.hpp"
#include "GuiEditBox.hpp"
#include "GuiCheckBox.hpp"
#include "GuiFrame.hpp"

using namespace Spiral;
using namespace Spiral::GUI;
using namespace boost;

GuiManager::GuiManager( Engine* engine ):
m_windowList(),
m_pImplEngine( engine )
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
				gfxDrvier->Set( BlendMode_t::SrcAlpha() );
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
	}else if( inputEvent.hasCat( Catagory_KeyBoard::value ) )
	{
		if( inputEvent.IsCat( Catagory_KeyBoard_Char::value ) )
		{
			HandleCharInput( inputEvent, boost::any_cast< uint32_t >( data ) );
		}
		
	}
}

void GuiManager::HandleMouseInput( const Event& inputEvent, const mouse_position& position )
{
	if( inputEvent.IsCat( Catagory_Mouse_MouseDown::value ) )
	{
		std::for_each( m_windowList.begin(), m_windowList.end(), boost::bind( &GuiWindow::MouseDown, _1, boost::cref(position) ) );
	}else
		if( inputEvent.IsCat( Catagory_Mouse_Up::value ) )
	{
		std::for_each( m_windowList.begin(), m_windowList.end(), boost::bind( &GuiWindow::MouseUp, _1, boost::cref(position) ) );
	}else
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



void GuiManager::Clear()
{
	m_windowList.clear();
	GuiWindow::lastWindow = NULL;
}

boost::shared_ptr< GuiButton > GuiManager::Make_DefButton( SpReal posX, SpReal posY, SpReal width, SpReal height )
{
	shared_ptr< Texture > button_texture = m_pImplEngine->LoadTexture( "Data/GUI/def_button.png", "Button_texture" );
	shared_ptr< GuiButton > button = make_shared< GuiButton >( Math::make_vector( posX,posY), Rect< SpReal >( 0, width, height, 0 ), 
		Rect< SpReal >( 0.0f, 0.5f, 1.0f, 0.0f ), button_texture, true );
	return button;
}

boost::shared_ptr< GuiWindow > GuiManager::Make_DefWindow( SpReal posX, SpReal posY, SpReal width, SpReal height )
{
	shared_ptr< Texture > window_texture = m_pImplEngine->LoadTexture( "Data/GUI/def_window_new.png", "Window_texture" );
	shared_ptr< GuiWindow > window = make_shared< GUI::GuiWindow >( Math::make_vector( posX, posY ), Rect< SpReal >( 0, width, height, 0 ),
		Rect< SpReal >( 0.0f, 1.0f, 1.0f, 0.0f ), window_texture, true );
	return window;
}

boost::shared_ptr< GuiCheckBox > GuiManager::Make_DefCheckBox( SpReal posX, SpReal posY, SpReal width, SpReal height )
{
	shared_ptr< Texture > check_boxTexture = m_pImplEngine->LoadTexture( "Data/GUI/def_check.png", "check_texture" );
	shared_ptr< GuiCheckBox > check = make_shared< GuiCheckBox >( Math::make_vector( posX, posY ),
		Rect< SpReal >( 0.0f, width, height, 0.0f ), Rect< SpReal >( 0.0f, 0.5f, 1.0f, 0.0f ), Rect< SpReal >( 0.5f, 1.0f, 1.0f, 0.0f ),
		check_boxTexture,check_boxTexture,true,true );

	return check;
}

boost::shared_ptr< GuiText > GuiManager::Make_DefTextBox( SpReal posX, SpReal posY, const Rgba& fontColor, const boost::shared_ptr<Font>& font, boost::uint32_t maxAllowedChar, const SpString& text )
{
	shared_ptr< GuiText > guiText = make_shared< GuiText >( Math::make_vector<SpReal>(posX, posY),m_pImplEngine->GetGfxDriver(),fontColor,maxAllowedChar,font,text,true );
	return guiText;
}

boost::shared_ptr< GuiSlider > GuiManager::Make_DefSlider( SpReal posX, SpReal posY, SpReal width, SpReal height, boost::uint32_t sliderSize, const GuiSliderDir& dir )
{
	shared_ptr< Texture > slider_bgTexture = m_pImplEngine->LoadTexture( "Data/GUI/def_slider_back.png", "slider_bk_texture" );
	shared_ptr< Texture > slider_texture = m_pImplEngine->LoadTexture( "Data/GUI/def_slider.png", "slider_texture" );

	shared_ptr< GuiSlider > guiSlider( new GuiSlider( Math::make_vector( posX, posY ), Rect<SpReal>( 0.0f, width, height, 0.0f ), 
		Rect<SpReal>( 0.0f,1.0f,1.0f,0.0f ), Rect<SpReal>(0.0f,1.0f,1.0f,0.0f),sliderSize, slider_bgTexture,
		slider_texture, dir, true, true ) );

	return guiSlider;
}

boost::shared_ptr< GuiEditBox > GuiManager::Make_DefEditBox( SpReal posX, SpReal posY, const Rgba& bkColor, const Rgba& fontColor, const boost::shared_ptr<Font>& font, boost::uint32_t maxCharLen, const SpString& text )
{
	shared_ptr< GuiEditBox > editbox = make_shared< GuiEditBox >( Math::make_vector( posX,posY ), m_pImplEngine->GetGfxDriver(), 
		bkColor, fontColor, font, maxCharLen, text );

	return editbox;
}

boost::shared_ptr< GuiFrame > GuiManager::Make_DefFrame( SpReal posX, SpReal posY, SpReal width, SpReal height )
{
	shared_ptr< Texture > window_texture = m_pImplEngine->LoadTexture( "Data/GUI/def_frame_window.png", "Frame_window_texture" );
	shared_ptr< GuiFrame > frame = make_shared< GuiFrame >( Math::make_vector( posX, posY ), Rect< SpReal >( 0, width, height, 0 ),
		Rect< SpReal >( 0.0f, 1.0f, 1.0f, 0.0f ), window_texture, true );
	return frame;
}
