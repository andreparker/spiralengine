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

using namespace Spiral;
using namespace Spiral::GUI;
using namespace boost;

GuiText::GuiText( const Math::SpVector2r& position, const boost::shared_ptr<GfxDriver>& gfxDriver, const Rgba& textColor,
				 boost::uint32_t maxCharLen, const boost::shared_ptr<Font>& font, const std::string& text ):
GuiWindow( position, Rect<SpReal>(), shared_ptr<Texture>(), true ), m_text( text ), m_charPos( 0 ),
m_font( font ),m_editSurface(),m_maxCharLen( maxCharLen ),m_fontColor( textColor )
{
	boost::int32_t width = font->GetCharWidth() * m_maxCharLen;
	boost::int32_t height = font->GetCharHeight();

	shared_ptr<Texture> textTexture;
	TextureInfo_t info;

	info.bitDepth = 32;
	info.bManaged = false;
	info.width = width;
	info.height = height;

	gfxDriver->CreateTexture( info, textTexture );
	m_editSurface = make_surfaceUP( textTexture->GetWidth(), textTexture->GetHeight(), 4, NULL );

	Rect<SpReal> texCoords( 0, static_cast<SpReal>(width)/textTexture->GetWidth(),
							 static_cast<SpReal>(height)/textTexture->GetHeight(), 0 );

	Rect<SpReal> rect( 0, static_cast<SpReal>(width), static_cast<SpReal>(height), 0 );

	SetRect( rect );
	SetTexCoords( texCoords );
	SetTexture( textTexture );
	SetText( text );
}

void GuiText::DrawString( const std::string& str )
{
	shared_ptr< Resource > res = GetTexture()->GetResource();

	ResLockRtInfo_t info;
	if( res->Lock( info, false ) )
	{
		SurfaceUP* surf = static_cast<SurfaceUP*>( m_editSurface.get() );
		surf->SetDataPtr( info.data );
		m_font->RenderAlpha( m_editSurface, m_charPos, str, m_fontColor );
		res->Unlock();
	}
}

void GuiText::DrawChar( char c )
{
	if( m_maxCharLen > m_text.length() )
	{
		m_text += c;
		m_charPos = 0;
		DrawString( m_text );
	}
}

void GuiText::SetText( const std::string& text )
{
	if( m_maxCharLen > m_text.length() )
	{
		m_text = text;
		m_charPos = 0;
		DrawString( m_text );
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