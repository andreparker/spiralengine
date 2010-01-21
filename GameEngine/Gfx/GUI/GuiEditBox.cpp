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

void Dummy( GfxDriver*& /*driver*/ ){}

GuiEditBox::GuiEditBox( const Math::Vector2f& position, const boost::shared_ptr<GfxDriver>& gfxDriver,const Rgba& backColor,
					    const Rgba& forColor, const boost::shared_ptr<Font>& font, boost::uint32_t maxCharLen, const wString& defText ):
GuiWindow( position, Rect<SpReal>(), shared_ptr<Texture>(), false ), m_textBox(),m_backColor(backColor),m_maxCharLen(maxCharLen)
{
//	Rect<SpReal> rect;
	
	ConnectHandler( char_input, bind( &GuiEditBox::OnChar, this, _1, _2, _3 ) );

	m_textBox = boost::make_shared< GuiText >();
	SetFontColor( forColor );
	SetFont( font, gfxDriver.get() );
	SetText( defText );
	// calculate the window size from our font
// 	int32_t width = maxCharLen * (font->GetCharWidth()/2);
// 	int32_t height = font->GetCharHeight();
// 
// 	rect.Set( 0, static_cast< SpReal >( width ), 0, static_cast< SpReal >( height ) );
// 	SetRect( rect );
// 
// 	shared_ptr<Surface> windowSurf = make_surface( width, height, 3 );
// 	
// 	// set background color of the text window
// 	windowSurf->Fill( backColor );
// 	
// 	shared_ptr<Texture> windowTex = windowSurf->CreateTextureFromData( gfxDriver );
// 	SetTexture( windowTex );
// 
// 	Rect<SpReal> backCoords( 0, static_cast<SpReal>(width)/windowTex->GetWidth(), 
// 								static_cast<SpReal>(height)/windowTex->GetHeight(), 0 );
// 	SetTexCoords( backCoords );
	
//  m_textBox = boost::make_shared<GuiText>( Math::make_vector<SpReal>(0.0f,0.0f), gfxDriver, forColor, maxCharLen, font, defText );
	
	AddChild( m_textBox );

	// show the window and also disable focus of it
	// the parent handles all events
	m_textBox->Show();
	m_textBox->AllowFocus(false);
	Show();
}

GuiEditBox::GuiEditBox():
GuiWindow( Math::make_vector< SpReal >( 0.0f, 0.0f ), 
		   Rect<SpReal>(), shared_ptr<Texture>(), false ), 
		   m_textBox(),m_backColor(),m_maxCharLen(16)
{
	ConnectHandler( char_input, bind( &GuiEditBox::OnChar, this, _1, _2, _3 ) );

	m_textBox = boost::make_shared< GuiText >();
	
	AddChild( m_textBox );

	m_textBox->Show();
	m_textBox->AllowFocus(false);
	Show();
}

void GuiEditBox::DrawString( const wString& str )
{
	m_textBox->SetText( str );
}

const wString& GuiEditBox::GetText() const
{
	return m_textBox->GetText();
}

void GuiEditBox::SetText( const wString& text )
{
	m_textBox->SetText( text );
}

void GuiEditBox::OnChar( boost::int32_t eventId, GuiWindow* window, const boost::any& data )
{
	// this will not work, function being called from a thread
	// opengl cannot render in different threads
	// TODO: add update function for post updates for GuiWindows 
	wChar c = static_cast<wChar>( any_cast<uint32_t>(data) );
	UpdateQueue_handle queue;

	if( c == 0x08 )
	{
		queue->Add( bind( &GuiText::EraseEnd, cref(m_textBox) ), 0.05f );
	}else
	{
		queue->Add( bind( &GuiText::DrawChar, cref(m_textBox), c ), 0.05f );
	}
	
}

void GuiEditBox::OnDataChanged( boost::int32_t eventId, GuiWindow* window, const boost::any& data )
{
	// rout the call
	CallHandler( data_changed, this, boost::any() );
}

void GuiEditBox::SetFont( const boost::shared_ptr< Font >& font, GfxDriver* gfxDriver )
{
	m_textBox->SetFont( font, gfxDriver );

	// calculate the window size from our font
	int32_t width = m_maxCharLen * (font->GetCharWidth()/2);
	int32_t height = font->GetCharHeight();

	boost::shared_ptr< GfxDriver > driver( gfxDriver, Dummy );
	Resize( width, height, driver );
}

void GuiEditBox::SetMaxCharLen( boost::uint32_t maxCharLen )
{
	m_textBox->SetMaxCharLen( maxCharLen );
}

void GuiEditBox::SetFontColor( const Rgba& color )
{
	m_textBox->SetFontColor( color );
}

void GuiEditBox::SetMultiLine( bool isMulti )
{ 
	m_textBox->SetMultiLine( isMulti );
}


void GuiEditBox::Resize( boost::int32_t width, boost::int32_t height, const boost::shared_ptr< GfxDriver >& gfxDriver )
{
	Rect<SpReal> rect;
	rect.Set( 0, static_cast< SpReal >( width ), 0, static_cast< SpReal >( height ) );
	SetRect( rect );

	shared_ptr<Surface> windowSurf = make_surface( width, height, 3 );

	// set background color of the text window
	windowSurf->Fill( m_backColor );
 
	shared_ptr<Texture> windowTex = windowSurf->CreateTextureFromData( gfxDriver );
	SetTexture( windowTex );

	Rect<SpReal> backCoords( 0, static_cast<SpReal>(width)/windowTex->GetWidth(), 
		static_cast<SpReal>(height)/windowTex->GetHeight(), 0 );
	SetTexCoords( backCoords );
}

void GuiEditBox::SetBkGroundColor( const Rgba& color )
{
	m_backColor = color;
	RefreshBackGround();
}

void GuiEditBox::RefreshBackGround()
{
	shared_ptr<Texture> tex = GetTexture();

	if( tex )
	{
		shared_ptr<Resource> res = tex->GetResource();

		ResLockRtInfo_t info;
		if( res->Lock( info, false ) )
		{
			SurfaceUP surf( info.rect.right, info.rect.bottom, 3, info.data );
			surf.Fill( m_backColor );

			res->Unlock();
		}
	}
	
}