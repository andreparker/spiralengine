#include "GuiCheckBox.hpp"
#include "GuiWindowEvents.hpp"
#include "../../Math/Math.hpp"

#include <boost/bind.hpp>
#include <boost/make_shared.hpp>

using namespace boost;
using namespace Spiral;
using namespace Spiral::GUI;

GuiCheckBox::GuiCheckBox( const Math::SpVector2r& position, const Rect< SpReal >& rect, const Rect< SpReal >& bgTextCoords,
						 const Rect< SpReal >& ckTextCoords, const boost::shared_ptr< Texture >& bgTexture,
						 const boost::shared_ptr< Texture >& ckTexture , bool bBgAlpha, bool bCkAlpha ):
GuiButton( position, rect, bgTextCoords, bgTexture, bBgAlpha ), m_isChecked( false ), m_ckWindow()
{
	ConnectHandler( button_Press, bind( &GuiCheckBox::CheckPressed, this, _1, _2, _3 ) );

	m_ckWindow = make_shared<GuiWindow>( Math::make_vector( 0.0f, 0.0f ), rect, ckTextCoords, ckTexture, bCkAlpha );
	m_ckWindow->Show( false );
	m_ckWindow->AllowFocus( false );

	AddChild( m_ckWindow );
}

GuiCheckBox::~GuiCheckBox()
{
	DisConnectHandler( button_Press );
}

void GuiCheckBox::CheckPressed( boost::int32_t /*eventId*/, GuiWindow* /*window*/, const boost::any& /*data*/ )
{
	SetCheck( !m_isChecked );
}

void GuiCheckBox::SetCheck( bool isChecked )
{
	m_isChecked = isChecked;
	m_ckWindow->Show( m_isChecked );
}