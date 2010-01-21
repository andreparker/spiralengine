#include <boost/make_shared.hpp>
#include "GuiText.hpp"
#include "../SurfaceUP.hpp"
#include "../Texture.hpp"
#include "../GfxImpl.hpp"
#include "../GfxDriver.hpp"
#include "../Font.hpp"
#include "../Color.hpp"
#include "../../Resource/Resource.hpp"
#include "../../Resource/ResLockImpl.hpp"
#include "GuiWindowEvents.hpp"

using namespace Spiral;
using namespace Spiral::GUI;
using namespace boost;

const boost::uint32_t kPadHeight = 0;
GuiText::GuiText( const Math::Vector2f& position, const boost::shared_ptr<GfxDriver>& gfxDriver, const Rgba& textColor,
				  boost::uint32_t maxCharLen, const boost::shared_ptr<Font>& font, const wString& text, bool multiline ):
GuiWindow( position, Rect<SpReal>(), shared_ptr<Texture>(), true ), m_text( text ), m_charPos( 0 ),
m_font( font ),m_editSurface(),m_maxCharLen( maxCharLen ),m_fontColor( textColor ),m_multiline(multiline),
m_gfxDriver( gfxDriver.get() )
{
	SetFont( font, gfxDriver.get() );
}

GuiText::GuiText():
GuiWindow( Math::make_vector<SpReal>(0,0) , Rect<SpReal>(), shared_ptr<Texture>(), true ),
m_text(),m_charPos(0),m_font(),m_editSurface(),m_maxCharLen(16),m_fontColor(),m_multiline(false),
m_gfxDriver(NULL)
{
}

void GuiText::DrawString( const wString& str )
{
	shared_ptr< Resource > res = GetTexture()->GetResource();

	ResLockRtInfo_t info;
	if( res->Lock( info, false ) )
	{
		SurfaceUP* surf = static_cast<SurfaceUP*>( m_editSurface.get() );
		surf->SetDataPtr( info.data );
		m_font->RenderAlpha( m_editSurface, m_charPos, str, m_fontColor );
		res->Unlock();

		// report that data has changed
		CallHandler( data_changed, this, boost::any() );
	}
}

void GuiText::DrawChar( wChar c )
{
	if( m_maxCharLen > m_text.length() )
	{
		m_text += c;
		m_charPos = 0;
		DrawString( m_text );
	}
}

void GuiText::SetText( const wString& text )
{
	if( m_multiline || m_maxCharLen > m_text.length() )
	{
		m_text = text;
		UpdateBox();
	}
}

void GuiText::EraseEnd()
{
	if( m_text.length() )
	{
		m_charPos = 0;
		m_text.erase( m_text.length()-1, 1 );
		ClearBox();
	}

	DrawString( m_text );
}

void GuiText::ClearBox()
{
	shared_ptr< Texture > texture = GetTexture();
	if( texture )
	{
		shared_ptr< Resource > res = GetTexture()->GetResource();

		ResLockRtInfo_t info;
		if( res->Lock( info, false ) )
		{
			SurfaceUP* surf = static_cast<SurfaceUP*>( m_editSurface.get() );
			surf->SetDataPtr( info.data );
			surf->Fill( Rgba( 0.0f, 0.0f, 0.0f, 0.0f ) );
			res->Unlock();
		}
	}
}

void GuiText::SetFont( const boost::shared_ptr< Font >& font, GfxDriver* gfxDriver )
{
	m_font = font;
	m_gfxDriver = gfxDriver;

	UpdateBox();
}


void GuiText::SetMaxCharLen( boost::uint32_t maxCharLen )
{
	
	if( !m_multiline && m_maxCharLen != maxCharLen && m_gfxDriver )
	{
		m_maxCharLen = maxCharLen;

		int32_t width = m_font->GetCharWidth() * m_maxCharLen;
		int32_t height = m_font->GetCharHeight() + kPadHeight;

		ResizeBox(width, height);
	}
}

void GuiText::ResizeBox( int32_t width, int32_t height )
{
	shared_ptr<Texture> textTexture;
	TextureInfo_t info;

	info.bitDepth = 32;
	info.bManaged = false;
	info.width = width;
	info.height = height;

	m_gfxDriver->CreateTexture( info, textTexture );
	m_editSurface = make_surfaceUP( textTexture->GetWidth(), textTexture->GetHeight(), 4, NULL );

	Rect<SpReal> texCoords( 0, static_cast<SpReal>(width)/textTexture->GetWidth(),
		static_cast<SpReal>(height)/textTexture->GetHeight(), 0 );

	Rect<SpReal> rect( 0, static_cast<SpReal>(width), static_cast<SpReal>(height), 0 );

	SetRect( rect );
	SetTexCoords( texCoords );
	SetTexture( textTexture );
	Refresh();
}

void GuiText::SetFontColor( const Rgba& color )
{
	m_fontColor = color;
	Refresh();
}

void GuiText::Refresh()
{
	if( m_font )
	{
		m_charPos = 0;
		ClearBox();
		DrawString( m_text );
	}
}

void GuiText::UpdateBox()
{
	if( m_font )
	{
		boost::int32_t width = 16;
		boost::int32_t height = 16;

		if( m_multiline && !m_text.empty() )
		{
			m_font->CalcSurfaceSize( m_text, width, height );
		}else
		{
			width = m_font->GetCharWidth() * m_maxCharLen;
			height = m_font->GetCharHeight() + kPadHeight;
		}

		ResizeBox( width, height );
	}
	
}