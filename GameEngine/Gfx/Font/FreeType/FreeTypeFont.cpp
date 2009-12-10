#include "FreeTypeFont.hpp"
#include <boost/assert.hpp>
#include <freetype/ftbitmap.h>
#include <boost/tokenizer.hpp>

#include "../../Surface.hpp"
#include "../../Color.hpp"

#include "../../../Core/TypeUtils.hpp"

using namespace Spiral;
using namespace boost;

const uint32_t kPadding = 4;

FreeTypeFont::~FreeTypeFont()
{

}

FreeTypeFont::FreeTypeFont( FT_Library library, FT_Face font,const boost::shared_array< boost::int8_t >& data, boost::int32_t width, boost::int32_t height ):
Font( width, ( font->size->metrics.height >> 6 ) ),m_library( library ), m_face( font ),m_data( data )
{

}

/*!
   @function  FindMaxStrSize
   @brief     finds the max string size, newlines in the string marks a sub string
   @return    boost::int32_t
   @param     const std::string & str
*/
boost::uint32_t FindMaxStrSize( const std::string& str )
{
	uint32_t firstPos = 0,lastPos = 0;
	uint32_t len = 0;

	tokenizer< char_separator<char> > tokens( str,char_separator<char>("\n") );
	for( tokenizer< char_separator<char> >::iterator itr = tokens.begin(); itr != tokens.end(); ++itr )
	{
		uint32_t curlen = (*itr).length();
		if( curlen > len )
			len = curlen;
	}

	return ( len == 0 ? str.length() : len );
}

void FreeTypeFont::DoCalcSurfaceSize( const std::string& str, boost::int32_t& surfWidth, boost::int32_t& surfHeight )
{
	surfWidth = FindMaxStrSize( str ) * (m_face->max_advance_width/2 >> 6 );
	surfHeight = GetCharHeight() * ( 1 + count_element_occurences( str.begin(), str.end(), '\n' ) ) + GetCharHeight();
}
																															  

struct SurfacePosition
{
	SurfacePosition( boost::int32_t x_, boost::int32_t y_ ):
	x( x_ ),y( y_ ){}
	SurfacePosition():x(0),y(0){}

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

struct TextData
{
	TextData( FT_Face face_, const boost::shared_ptr< Surface >& surface, const SurfacePosition& pos ):
	face( face_ ),renderSurface( surface ),cursorPos( pos ){}
	
	FT_Face face;
	boost::shared_ptr< Surface > renderSurface;
	SurfacePosition cursorPos;
};

template< void Render( const SurfacePosition&, const FT_Bitmap&, const Rgba&, boost::shared_ptr< Surface >& ) >
void DrawText( TextData& data, const std::string& str, const Rgba& color )
{
	const char* text = str.c_str();

	SurfacePosition pos( data.cursorPos.x, ( data.face->size->metrics.height >> 6 ) - kPadding );
	bool useKerning = bool( FT_HAS_KERNING( data.face ) ? true : false );
	FT_UInt preIndex = 0;

	while( *text != 0 )
	{
		char c = *text;

		if( c == '\n' )
		{
			pos.y += ( data.face->size->metrics.height >> 6 );
			pos.x = 0;
			++text;
			continue;
		}

		FT_UInt index = FT_Get_Char_Index( data.face, c );
		FT_Error result = FT_Load_Glyph( data.face, index, FT_LOAD_DEFAULT );
		if( !result )
		{
			if( useKerning && index && preIndex )
			{
				FT_Vector delta;

				FT_Get_Kerning( data.face, preIndex, index, 0, &delta );

				pos.x += delta.x >> 6;
			}

			result = FT_Render_Glyph( data.face->glyph, FT_RENDER_MODE_NORMAL );
			FT_GlyphSlot slot = data.face->glyph;
			if( !result )
			{
				SurfacePosition tmpPos( pos.x + slot->bitmap_left, pos.y - slot->bitmap_top  );
				Render( tmpPos, slot->bitmap, color, data.renderSurface );
			}

			pos.x += data.face->glyph->advance.x >> 6;  // advance is in 26.6 format, convert to int
			preIndex = index;
		}

		++text;
	}

	data.cursorPos.x = pos.x;
	data.cursorPos.y = pos.y - (( data.face->size->metrics.height >> 6 )) ;
}

void FreeTypeFont::DoRenderAlpha( boost::shared_ptr< Surface >& surface, const std::string& str, const Rgba& color )
{
	TextData renderInfo( m_face, surface, SurfacePosition() );
	DrawText< RenderBitmapAlpha >( renderInfo, str, color );
}

void FreeTypeFont::DoRenderOpaque( boost::shared_ptr< Surface >& surface, const std::string& str, const Rgba& color )
{
	TextData renderInfo( m_face, surface, SurfacePosition() );
	DrawText< RenderBitmapOpaque >( renderInfo, str, color );
}

void FreeTypeFont::DoRenderAlpha( boost::shared_ptr< Surface >& surface, boost::uint32_t& cursorX, const std::string& str, const Rgba& color )
{
	TextData renderInfo( m_face, surface, SurfacePosition(cursorX,0) );
	DrawText< RenderBitmapAlpha >( renderInfo, str, color );
	cursorX = renderInfo.cursorPos.x;
}

void FreeTypeFont::DoRenderOpaque( boost::shared_ptr< Surface >& surface, boost::uint32_t& cursorX, const std::string& str, const Rgba& color )
{
	TextData renderInfo( m_face, surface, SurfacePosition(cursorX,0) );
	DrawText< RenderBitmapOpaque >( renderInfo, str, color );
	cursorX = renderInfo.cursorPos.x;
}