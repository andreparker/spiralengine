#include "GuiButton.hpp"
#include "GuiWindowEvents.hpp"
#include "../Texture.hpp"
#include "../../Core/Engine.hpp"
#include "../../Script/ScriptManager.hpp"
#include "Gui.hpp"

#include <boost/bind.hpp>


using namespace Spiral;
using namespace Spiral::GUI;
using namespace boost;

GuiButton::GuiButton( const Math::Vector2f& position, const Rect< SpReal >& rect, const Rect< SpReal >& textCoords,const boost::shared_ptr< Texture >& texture, bool bAlpha ):
GuiWindow( position, rect, textCoords, texture, bAlpha ),
m_buttonDown(false),m_hoverCoordsSet(false),m_buttonPressScript(),m_gui(NULL)
{
	SetupHandlers();
}

GuiButton::GuiButton():
GuiWindow( Math::make_vector<SpReal>( 0,0 ), Rect<SpReal>(), Rect<SpReal>(), shared_ptr<Texture>(), false ),
m_buttonDown(false),m_hoverCoordsSet(false),m_buttonPressScript(),m_gui(NULL)
{
	SetupHandlers();
}

GuiButton::~GuiButton()
{
	DisConnectHandler( mouse_down );
	DisConnectHandler( mouse_up );
	DisConnectHandler( mouse_hover );
}

void GuiButton::SetupHandlers()
{
	ConnectHandler( mouse_down, bind( &GuiButton::OnMouseDown, this, _1, _2, _3 ) );
	ConnectHandler( mouse_up, bind( &GuiButton::OnMouseUp, this, _1, _2, _3 ) );
	ConnectHandler( mouse_hover, bind( &GuiButton::OnMouseHover, this, _1, _2, _3 ) );
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
		OnButtonPressScript();                    // call a script
		m_buttonDown = false;
	}
}

void GuiButton::OnMouseHover( boost::int32_t /*eventId*/, GuiWindow* /*window*/, const boost::any& /*data*/ )
{
	if( m_hoverCoordsSet == false )
	{
		SetTexCoords( Rect< SpReal >( 0.5f, 1.0f, 1.0f, 0.0f ) );
		m_hoverCoordsSet = true;
	}
}

void GuiButton::ResetWindow()
{
	SetTexCoords( Rect< SpReal >( 0.0f, 0.5f, 1.0f, 0.0f ) );
	m_hoverCoordsSet = false;
	m_buttonDown = false;
}


void GuiButton::OnButtonPressScript()
{
	if( m_gui && m_buttonPressScript != "" )
	{
		shared_ptr< ScriptManager > scriptMgr = m_gui->GetEngine()->GetScriptManager();
		luabind::call_function<void>( scriptMgr->GetLuaState(), m_buttonPressScript.c_str(), this );
	}
}