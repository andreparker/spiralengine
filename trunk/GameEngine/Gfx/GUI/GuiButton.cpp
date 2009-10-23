#include "GuiButton.hpp"
#include "GuiWindowEvents.hpp"
#include <boost/bind.hpp>

using namespace Spiral;
using namespace Spiral::GUI;
using namespace boost;

GuiButton::GuiButton( const Math::SpVector2r& position, const Rect< SpReal >& rect, const Rect< SpReal >& textCoords,const boost::shared_ptr< Texture >& texture, bool bAlpha ):
GuiWindow( position, rect, textCoords, texture, bAlpha ),
m_buttonDown(false)
{
	ConnectHandler( mouse_down, bind( &GuiButton::OnMouseDown, this, _1, _2, _3 ) );
	ConnectHandler( mouse_up, bind( &GuiButton::OnMouseUp, this, _1, _2, _3 ) );
	ConnectHandler( mouse_hover, bind( &GuiButton::OnMouseHover, this, _1, _2, _3 ) );
}

GuiButton::~GuiButton()
{
	DisConnectHandler( mouse_down );
	DisConnectHandler( mouse_up );
	DisConnectHandler( mouse_hover );
}

void GuiButton::OnMouseDown( boost::int32_t /*eventId*/, GuiWindow* /*window*/, const boost::any& /*data*/ )
{
	m_buttonDown = true;
}

void GuiButton::OnMouseUp( boost::int32_t /*eventId*/, GuiWindow* /*window*/, const boost::any& data )
{
	if( m_buttonDown )
	{
		CallHandler( button_Press, this, data );  // pass the button press event
		m_buttonDown = false;
	}
}

void GuiButton::OnMouseHover( boost::int32_t /*eventId*/, GuiWindow* /*window*/, const boost::any& /*data*/ )
{

}

void GuiButton::ResetWindow()
{
	m_buttonDown = false;
}