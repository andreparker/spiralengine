#include "Font.hpp"
#include "Surface.hpp"
#include "Color.hpp"

using namespace Spiral;

Font::~Font()
{

}

void Font::CalcSurfaceSize( const SpString& str, boost::int32_t& surfWidth, boost::int32_t& surfHeight )
{
	DoCalcSurfaceSize( str, surfWidth, surfHeight );
}

void Font::RenderAlpha( boost::shared_ptr< Surface >& surface, const SpString& str, const Rgba& color )
{
	DoRenderAlpha( surface, str, color );
}

void Font::RenderOpaque( boost::shared_ptr< Surface >& surface, const SpString& str, const Rgba& color )
{
	DoRenderOpaque( surface, str, color );
}

Font::Font( boost::int32_t width, boost::int32_t height ):
m_charWidth( width ), m_charHeight( height )
{

}

void Font::RenderAlpha( boost::shared_ptr< Surface >& surface, boost::uint32_t& cursorX, const SpString& str, const Rgba& color )
{
	DoRenderAlpha( surface, cursorX, str, color );
}

void Font::RenderOpaque( boost::shared_ptr< Surface >& surface, boost::uint32_t& cursorX, const SpString& str, const Rgba& color )
{
	DoRenderOpaque( surface, cursorX, str, color );
}

bool Font::SetFontSize( boost::int32_t width, boost::int32_t height )
{
	return DoSetSize( width, height );
}