#include "FreeTypeFont.hpp"
#include <boost/assert.hpp>
#include <freetype/ftbitmap.h>

#include "../../Surface.hpp"
#include "../../Color.hpp"

#include "../../../Core/TypeUtils.hpp"

using namespace Spiral;
using namespace boost;

FreeTypeFont::~FreeTypeFont()
{

}

FreeTypeFont::FreeTypeFont( FT_Library library, FT_Face font,const boost::shared_array< boost::int8_t >& data, boost::int32_t width, boost::int32_t height ):
Font( width, height ),m_library( library ), m_face( font ),m_data( data )
{

}

void FreeTypeFont::DoCalcSurfaceSize( const std::string& str, boost::int32_t& surfWidth, boost::int32_t& surfHeight )
{
	surfWidth = str.size() * GetCharWidth();
	surfHeight = GetCharHeight() * count_element_occurences( str.begin(), str.end(), '\n' );
}
																															  

struct SurfacePosition
{
	boost::int32_t x,y;
};

inline void StorePixelRgba( unsigned char* currentLine, const Rgba& color, unsigned char* bitmap_surf )
{
	currentLine[0] = color.GetByteComponent( 0 );
	currentLine[1] = color.GetByteComponent( 1 );
	currentLine[2] = color.GetByteComponent( 2 );
	currentLine[3] = *bitmap_surf;
}

inline void StorePixelRgb( unsigned char* currentLine, const Rgba& color, unsigned char* bitmap_surf )
{
	currentLine[0] = static_cast< unsigned char >( color[0] * (*bitmap_surf) );
	currentLine[1] = static_cast< unsigned char >( color[1] * (*bitmap_surf) );
	currentLine[2] = static_cast< unsigned char >( color[2] * (*bitmap_surf) );
}

template< void PixelStorePolicy( unsigned char*, const Rgba&, unsigned char*), int32_t elementMinSize >
void RenderBitmapToSurf( const SurfacePosition& position, const FT_Bitmap& bitmap, const Rgba& color, boost::shared_ptr< Surface >& surface )
{
	int32_t rows = bitmap.rows;
	int32_t surfacePitch = surface->GetBytesPerLine();
	int32_t elementSize = surface->GetElementSize();

	BOOST_ASSERT( elementSize >= elementMinSize );
	unsigned char* surf = reinterpret_cast<unsigned char*>( surface->GetData() ) + position.y * surfacePitch + position.x * elementSize;
	unsigned char* bitmap_surf = bitmap.buffer;

	int32_t col = bitmap.width;
	unsigned char* currentSurfLine = surf;
	unsigned char* currentBitMapLine = bitmap.buffer;

	while( rows-- != 0 ) 
	{
		do 
		{
			if( *currentBitMapLine != 0 )
			{
				// store the pixel
				PixelStorePolicy(currentSurfLine, color, currentBitMapLine);
			}

			currentSurfLine += elementSize;
			++currentBitMapLine;
		} while ( --col != 0 );

		surf += surfacePitch;
		bitmap_surf += bitmap.pitch;
		col = bitmap.width;
		currentSurfLine = surf;
		currentBitMapLine = bitmap_surf;
	}
}

void RenderBitmapOpaque( const SurfacePosition& pos, const FT_Bitmap& bitmap, const Rgba& color, boost::shared_ptr< Surface >& surface )
{
	RenderBitmapToSurf< StorePixelRgb, 3 >( pos, bitmap, color, surface );
}

void RenderBitmapAlpha( const SurfacePosition& pos, const FT_Bitmap& bitmap, const Rgba& color, boost::shared_ptr< Surface >& surface )
{
	RenderBitmapToSurf< StorePixelRgba, 4 >( pos, bitmap, color, surface );
}

void FreeTypeFont::DoRenderAlpha( boost::shared_ptr< Surface >& surface, const std::string& str, const Rgba& color )
{
	const char* text = str.c_str();

	SurfacePosition pos = {0,0};

	while( *text != 0 )
	{
		char c = *text;

		if( c == '\n' )
		{
			pos.y += GetCharHeight();
			pos.x = 0;
			++text;
			continue;
		}

		FT_UInt index = FT_Get_Char_Index( m_face, c );
		FT_Error result = FT_Load_Glyph( m_face, index, FT_LOAD_DEFAULT );
		if( !result )
		{

			result = FT_Render_Glyph( m_face->glyph, FT_RENDER_MODE_NORMAL );
			FT_GlyphSlot slot = m_face->glyph;
			if( !result )
			{
				SurfacePosition tmpPos = { pos.x + slot->bitmap_left, pos.y + (GetCharHeight()-slot->bitmap_top) };
				RenderBitmapAlpha( tmpPos, slot->bitmap, color, surface );
			}

			pos.x += m_face->glyph->advance.x >> 6;  // advance is in 26.6 format, convert to int 
		}else
		{
			pos.x += GetCharWidth();
		}

		
		++text;
	}
}

void FreeTypeFont::DoRenderOpaque( boost::shared_ptr< Surface >& surface, const std::string& str, const Rgba& color )
{
	const char* text = str.c_str();

	SurfacePosition pos = {0,0};

	while( text != NULL )
	{
		char c = *text;

		if( c == '\n' )
		{
			pos.y += GetCharHeight();
			pos.x = 0;
			++text;
			continue;
		}

		FT_Error result = FT_Load_Char( m_face, c, FT_LOAD_DEFAULT );
		if( !result )
		{
			result = FT_Render_Glyph( m_face->glyph, FT_RENDER_MODE_NORMAL );
			FT_GlyphSlot slot = m_face->glyph;
			if( !result )
			{
				FT_Bitmap aligned;
				//SurfacePosition tmpPos = { pos.x + slot->bitmap_left, pos.y + slot->bitmap_top };
				FT_Bitmap_Convert( m_library, &m_face->glyph->bitmap, &aligned, 1 );
				RenderBitmapOpaque( pos, aligned, color, surface );
				FT_Bitmap_Done( m_library, &aligned );
			}
		}

		pos.x += GetCharWidth();
		++text;
	}
}