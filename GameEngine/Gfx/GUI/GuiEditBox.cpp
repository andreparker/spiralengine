#include "GuiEditBox.hpp"
#include "../Font.hpp"
#include "../GfxDriver.hpp"
#include "../Texture.hpp"
#include "../Surface.hpp"
#include "../GfxImpl.hpp"
#include "../../Resource/Resource.hpp"
#include "../../Resource/ResLockImpl.hpp"
#include "../SurfaceUP.hpp"
#include "GuiText.hpp"

using namespace Spiral;
using namespace Spiral::GUI;
using namespace boost;


GuiEditBox::GuiEditBox( const Math::SpVector2r& position, const boost::shared_ptr<GfxDriver>& gfxDriver,const Rgba& backColor,
					    const Rgba& forColor, const boost::shared_ptr<Font>& font, boost::uint32_t maxCharLen, const std::string& defText ):
GuiWindow( position, Rect<SpReal>(), shared_ptr<Texture>(), false ), m_textBox()
{
	Rect<SpReal> rect;
	
	// calculate the window size from our font
	int32_t width = maxCharLen * font->GetCharWidth();
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
	
	// create the edit texture and display window for the text
// 	TextureInfo_t info;
// 	info.bitDepth = 32;
// 	info.bManaged = false;
// 	info.width    = width;
// 	info.height   = height;
// 
// 	gfxDriver->CreateTexture( info, m_editTexture );
// 
// 	m_editSurface = make_surfaceUP( m_editTexture->GetWidth(), m_editTexture->GetHeight(), 4, NULL );
// 
// 	// adjust textcoords to fit window if texture is adjusted to a power of 2
// 	
// 	Rect<SpReal> textCoords( 0, static_cast<SpReal>(width)/m_editTexture->GetWidth(), 
// 							    static_cast<SpReal>(height)/m_editTexture->GetHeight(), 0 );
// 
// 
// 	shared_ptr<GuiWindow> textWindow = GuiWindow::Create( Math::make_vector<SpReal>(0.0f,0.0f), rect, 
// 														  textCoords, m_editTexture, true );
	AddChild( m_textBox );

	// show the window and also disable focus of it
	// the parent handles all events
	m_textBox->Show();
	m_textBox->AllowFocus(false);

	Show();
}

void GuiEditBox::DrawString( const std::string& str )
{
	m_textBox->DrawString( str );
}

const std::string& GuiEditBox::GetText() const
{
	return m_textBox->GetText();
}

void GuiEditBox::SetText( const std::string& text )
{
	m_textBox->SetText( text );
}