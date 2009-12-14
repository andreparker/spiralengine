#include "GuiEditBox.hpp"
#include "../Font.hpp"
#include "../GfxDriver.hpp"
#include "../Texture.hpp"
#include "../Surface.hpp"
#include "../GfxImpl.hpp"
#include "../../Resource/Resource.hpp"
#include "../../Resource/ResLockImpl.hpp"
#include "../SurfaceUP.hpp"
#include "GuiWindowEvents.hpp"
#include "GuiText.hpp"
#include "../../Core/UpdateQueue.hpp"

#include <boost/bind.hpp>

using namespace Spiral;
using namespace Spiral::GUI;
using namespace boost;


GuiEditBox::GuiEditBox( const Math::SpVector2r& position, const boost::shared_ptr<GfxDriver>& gfxDriver,const Rgba& backColor,
					    const Rgba& forColor, const boost::shared_ptr<Font>& font, boost::uint32_t maxCharLen, const SpString& defText ):
GuiWindow( position, Rect<SpReal>(), shared_ptr<Texture>(), false ), m_textBox()
{
	Rect<SpReal> rect;
	
	ConnectHandler( char_input, bind( &GuiEditBox::OnChar, this, _1, _2, _3 ) );
	
	// calculate the window size from our font
	int32_t width = maxCharLen * (font->GetCharWidth()/2);
	int32_t height = font->GetCharHeight();

	rect.Set( 0, static_cast< SpReal >( width ), 0, static_cast< SpReal >( height ) );
	SetRect( rect );

	shared_ptr<Surface> windowSurf = make_surface( width, height, 3 );
	
	// set background color of the text window
	windowSurf->Fill( backColor );
	
	shared_ptr<Texture> windowTex = windowSurf->CreateTextureFromData( gfxDriver );
	SetTexture( windowTex );

	Rect<SpReal> backCoords( 0, static_cast<SpReal>(width)/windowTex->GetWidth(), 
								static_cast<SpReal>(height)/windowTex->GetHeight(), 0 );
	SetTexCoords( backCoords );
	
    m_textBox = make_shared<GuiText>( Math::make_vector<SpReal>(0.0f,0.0f), gfxDriver, forColor, maxCharLen, font, defText );
	
	AddChild( m_textBox );

	// show the window and also disable focus of it
	// the parent handles all events
	m_textBox->Show();
	m_textBox->AllowFocus(false);

	Show();
}

void GuiEditBox::DrawString( const SpString& str )
{
	m_textBox->SetText( str );
}

const SpString& GuiEditBox::GetText() const
{
	return m_textBox->GetText();
}

void GuiEditBox::SetText( const SpString& text )
{
	m_textBox->SetText( text );
}

void GuiEditBox::OnChar( boost::int32_t eventId, GuiWindow* window, const boost::any& data )
{
	// this will not work, function being called from a thread
	// opengl cannot render in different threads
	// TODO: add update function for post updates for GuiWindows 
	SpChar c = any_cast<uint32_t>(data);
	UpdateQueue_handle queue;

	if( c == 0x08 )
	{
		queue->Add( bind( &GuiText::EraseEnd, cref(m_textBox) ), 0.05f );
	}else
	{
		queue->Add( bind( &GuiText::DrawChar, cref(m_textBox), c ), 0.05f );
	}
	
}