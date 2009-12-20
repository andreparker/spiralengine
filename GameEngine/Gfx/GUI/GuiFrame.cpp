#include "GuiFrame.hpp"
#include <boost/make_shared.hpp>
#include <boost/bind.hpp>

#include "../../Math/Math.hpp"
#include "GuiWindowEvents.hpp"

using namespace Spiral;
using namespace Spiral::GUI;

GuiFrame::GuiFrame( const Math::SpVector2r& position, const Rect< SpReal >& rect, const Rect< SpReal >& textCoords, 
				   const boost::shared_ptr< Texture >& texture, bool bAlpha ): 
GuiWindow( position, rect, textCoords, texture, bAlpha ), m_frameBar(),m_posX( 0.0f ), m_posY( 0.0f ),m_mouseDown( false )
{
	m_frameBar = boost::make_shared< GuiWindow >
		        ( Math::make_vector( 0.0f, 0.0f ), 
				  Rect<SpReal>( 0.0f, rect.right, 30.0f, 0.0f ),
				  textCoords, boost::shared_ptr< Texture >(),
				  bAlpha );

	m_frameBar->Show();

	m_frameBar->ConnectHandler( mouse_down, boost::bind( &GuiFrame::OnMouseDownBar, this, _1, _2, _3 ) );
	m_frameBar->ConnectHandler( mouse_up, boost::bind( &GuiFrame::OnMouseUpBar, this, _1, _2, _3 ) );
	m_frameBar->ConnectHandler( mouse_hover, boost::bind( &GuiFrame::OnHoverBar, this, _1, _2, _3 ) );

	AddChild( m_frameBar );
}

void GuiFrame::OnMouseDownBar( boost::int32_t eventId, GuiWindow* window, const boost::any& data )
{
	const mouse_position pos = boost::any_cast< const mouse_position >( data );
	m_mouseDown = true;

	SaveLastPosition( pos );
}

void GuiFrame::OnHoverBar( boost::int32_t eventId, GuiWindow* window, const boost::any& data )
{

	if( m_mouseDown )
	{
		const mouse_position pos = boost::any_cast< const mouse_position >( data );
		
		SpReal deltaX = pos.x - m_posX;
		SpReal deltaY = pos.y - m_posY;

		Math::SpVector2r windowPos = GetLocalPosition();
		windowPos[0] += deltaX;
		windowPos[1] += deltaY;
		SetLocalPosition( windowPos );
		SaveLastPosition( pos );
		
	}
}

void GuiFrame::OnMouseUpBar( boost::int32_t eventId, GuiWindow* window, const boost::any& data )
{
	m_mouseDown = false;
}

void GuiFrame::ResetWindow()
{
	m_mouseDown = false;
}

void GuiFrame::SaveLastPosition( const mouse_position& pos )
{
	m_posX = pos.x;
	m_posY = pos.y;
}