#include "Font.hpp"
#include "Surface.hpp"
#include "Color.hpp"

using namespace Spiral;

Font::~Font()
{

}

void Font::CalcSurfaceSize( const std::string& str, boost::int32_t& surfWidth, boost::int32_t& surfHeight )
{
	DoCalcSurfaceSize( str, surfWidth, surfHeight );
}

void Font::RenderAlpha( boost::shared_ptr< Surface >& surface, const std::string& str, const Rgba& color )
{
	DoRenderAlpha( surface, str, color );
}

void Font::RenderOpaque( boost::shared_ptr< Surface >& surface, const std::string& str, const Rgba& color )
{
	DoRenderOpaque( surface, str, color );
}

Font::Font( boost::int32_t width, boost::int32_t height ):
m_charWidth( width ), m_charHeight( height )
{

}