#include "GuiFrame.hpp"
#include <boost/make_shared.hpp>
#include <boost/bind.hpp>

#include "../../Math/Math.hpp"
#include "GuiWindowEvents.hpp"

using namespace Spiral;
using namespace Spiral::GUI;

GuiFrame::GuiFrame():
GuiWindow( Math::make_vector<SpReal>(0,0), Rect<SpReal>(),Rect<SpReal>(),boost::shared_ptr<Texture>(), false ),
m_frameBar(),m_posX( 0.0f ), m_posY( 0.0f ),m_mouseDown( false )
{
	CreateFrameBar( Rect<SpReal>(), Rect<SpReal>(), false );
}

GuiFrame::GuiFrame( const Math::Vector2f& position, const Rect< SpReal >& rect, const Rect< SpReal >& textCoords, 
				   const boost::shared_ptr< Texture >& texture, bool bAlpha ): 
GuiWindow( position, rect, textCoords, texture, bAlpha ), m_frameBar(),m_posX( 0.0f ), m_posY( 0.0f ),m_mouseDown( false )
{
	CreateFrameBar(rect, textCoords, bAlpha);
}


void GuiFrame::CreateFrameBar( const Rect< SpReal > &rect, const Rect< SpReal >& textCoords, bool bAlpha )
{
	m_frameBar = boost::make_shared< GuiWindow >
		( Math::make_vector( 0.0f, 0.0f ), 
		Rect<SpReal>( 0.0f, rect.right, 30.0f, 0.0f ),
		textCoords, boost::shared_ptr< Texture >(),
		bAlpha );

	m_frameBar->Show();

	m_frameBar->ConnectHandler( mouse_down, boost::bind( &GuiFrame::OnMouseDownBar, this, _1, _2, _3 ) );
	m_frameBar->ConnectHandler( mouse_up, boost::bind( &GuiFrame::OnMouseUpBar, this, _1, _2, _3 ) );
	m_frameBar->ConnectHandler( mouse_move, boost::bind( &GuiFrame::OnMouseMove, this, _1, _2, _3 ) );

	AddChild( m_frameBar );
}


void GuiFrame::SetRect( const Rect< SpReal >& rt )
{
	GuiWindow::SetRect( rt );
	m_frameBar->SetRect( Rect<SpReal>( 0.0f, rt.right, 30.0f, 0.0f ) );
}

void GuiFrame::OnMouseDownBar( boost::int32_t eventId, GuiWindow* window, const boost::any& data )
{
	const mouse_position pos = boost::any_cast< const mouse_position >( data );
	m_mouseDown = true;

	SaveLastPosition( pos );
}


void GuiFrame::OnMouseUpBar( boost::int32_t eventId, GuiWindow* window, const boost::any& data )
{
	m_mouseDown = false;
}

void GuiFrame::ResetWindow()
{
	//m_mouseDown = false;
}

void GuiFrame::SaveLastPosition( const mouse_position& pos )
{
	Math::Vector2f windowPos = GetLocalPosition();

	m_posX = pos.x - windowPos[0];
	m_posY = pos.y - windowPos[1];
}

void GuiFrame::OnMouseMove( boost::int32_t eventId, GuiWindow* window, const boost::any& data )
{
	if( m_mouseDown )
	{
		const mouse_position pos = boost::any_cast< const mouse_position >( data );
		Math::Vector2f windowPos = GetLocalPosition();

		SpReal distX = pos.x - windowPos[0];
		SpReal distY = pos.y - windowPos[1];


		windowPos[0] += (distX - m_posX);
		windowPos[1] += (distY - m_posY);
		SetLocalPosition( windowPos );

	}
}

