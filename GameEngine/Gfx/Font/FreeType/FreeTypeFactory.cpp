#include "FreeTypeFactory.hpp"
#include "FreeTypeFont.hpp"
#include <boost/scoped_array.hpp>
#include <boost/make_shared.hpp>

using namespace Spiral;
using namespace boost;

FreeTypeFactory::FreeTypeFactory():
m_library(NULL)
{}

FreeTypeFactory::~FreeTypeFactory()
{
	FT_Done_FreeType( m_library );
}

bool FreeTypeFactory::DoInitialize()
{
	FT_Error result = FT_Init_FreeType( &m_library );

	return bool( !result );
}

boost::shared_ptr< Font > FreeTypeFactory::DoCreateFont( const boost::shared_ptr< IFile >& fontFile, boost::int32_t charWidth, boost::int32_t charHeight )
{
	boost::shared_ptr< Font > font;
	if( fontFile && m_library )
	{
		FT_Face newFace;
		uint32_t fileSize = fontFile->Size();
		boost::shared_array< int8_t > buffer( new int8_t[ fileSize ] );

		fontFile->Read( buffer.get(), fileSize );

		FT_Error result = FT_New_Memory_Face( m_library, reinterpret_cast< const FT_Byte*>( buffer.get() ), fileSize, 0, &newFace );

		if( !result )
		{
			result = FT_Set_Pixel_Sizes( newFace, charWidth, charHeight );
			if( !result )
			{
				font = make_shared< FreeTypeFont >( m_library, newFace, buffer, charWidth, charHeight );
			}
		}
	}

	return font;
}