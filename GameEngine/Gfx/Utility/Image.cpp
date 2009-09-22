#include <boost/make_shared.hpp>
#include <algorithm>

#include "Image.hpp"
#include "../../Core/File.hpp"
#include "../../../ThirdParty/libpng/png.h"

using namespace Spiral::GfxUtil;
using namespace Spiral;
using namespace boost;

void cdecl CustomPngReadFunc( png_structp pngPtr, png_bytep data, png_size_t size )
{
	IFile* pngFile = static_cast< IFile* >( png_get_io_ptr( pngPtr ) );
	pngFile->Read( reinterpret_cast<int8_t*>( data ), size );
}

const int32_t Png_Check_Bytes = 8;

shared_ptr< Image > Image::LoadPng( boost::shared_ptr<IFile>& pngFile )
{
	int8_t signature[ Png_Check_Bytes ];
	shared_ptr< Image > image;

	// check to see if this is valid png file
	pngFile->Read( signature, Png_Check_Bytes );
	int isPng = !png_sig_cmp( reinterpret_cast<png_bytep>( signature ), 0, Png_Check_Bytes );

	if( isPng )
	{
		png_structp pngStructPtr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
		png_infop pngInfoPtr = png_create_info_struct( pngStructPtr );
		
		png_set_read_fn(pngStructPtr, reinterpret_cast<void*>(pngFile.get()), CustomPngReadFunc );
		png_set_sig_bytes( pngStructPtr, Png_Check_Bytes );

		png_uint_32 width,height;
		int bitDepth,colorType, interlaced;

		png_read_info( pngStructPtr, pngInfoPtr );
		png_get_IHDR( pngStructPtr, pngInfoPtr, &width, &height, &bitDepth, &colorType, &interlaced, NULL, NULL );

		if( bitDepth == 8 )
		{
			if( colorType == PNG_COLOR_TYPE_RGB )
			{
				bitDepth = 24;
			}else if( colorType == PNG_COLOR_TYPE_RGBA )
			{
				bitDepth = 32;
			}

			if( bitDepth == 32 || bitDepth == 24 )
			{
				int rowbytes = png_get_rowbytes( pngStructPtr, pngInfoPtr );
				png_bytepp rowsPtr = new png_bytep[ height ];

				// this is terrible, but its the way pnglib does things
				// I have to allocate rows instead of one big allocation...
				for( png_uint_32 i = 0; i < height; ++i )
				{
					rowsPtr[ i ] = new png_byte[ rowbytes ];
				}

				int8_t* data = new int8_t[ height * rowbytes ];

				png_read_image( pngStructPtr, rowsPtr );
				png_read_end( pngStructPtr, pngInfoPtr );

				for( png_uint_32 i = 0; i < height; ++i )
				{
					std::copy( rowsPtr[i], rowsPtr[i]+rowbytes, reinterpret_cast<png_bytep>(&data[i*rowbytes]) );
					delete [] rowsPtr[i];
				}

				delete [] rowsPtr;

				image = make_shared< Image >( width, height, bitDepth, data );
			}
		}
		
		// free memory used by pnglib
		png_destroy_read_struct( &pngStructPtr, &pngInfoPtr, png_infopp_NULL );
	}

	return image;
}

inline void copy4( const boost::int8_t* src, boost::int8_t* dst )
{
	boost::int32_t* dst4 = reinterpret_cast<boost::int32_t*>( dst );
	*dst4 = *reinterpret_cast<const boost::int32_t*>( src );
}

inline void copy3( const boost::int8_t* src, boost::int8_t* dst )
{
	*dst++ = *src++;
	*dst++ = *src++;
	*dst++ = *src++;
}


template< void copyElement( const boost::int8_t*, boost::int8_t* ), int colorChannels >
void BlitImpl( const Image::ImageDesc& src, Image::ImageDesc& dest, boost::int32_t x, boost::int32_t y )
{
	int8_t* destSurf = dest.data + (y * dest.rowBytes + x * dest.colorChannel);
	const int8_t* srcSurf = src.data;
	int32_t rows = src.height;
	int32_t col = src.width;

	int8_t* destline = destSurf;
	const int8_t* srcline = srcSurf;

	while( rows-- != 0 )
	{
		do 
		{
			copyElement( srcline, destline );
			srcline += colorChannels;
			destline += colorChannels;
		} while ( --col != 0 );

		srcSurf += src.rowBytes;
		destSurf += dest.rowBytes;

		destline = destSurf;
		srcline = srcSurf;

		col = src.width;
	}
}

void Image::Blit( const Image::ImageDesc& src, Image::ImageDesc& dest, boost::int32_t x, boost::int32_t y )
{
	if( src.colorChannel == dest.colorChannel )
	{
		if( src.colorChannel == 4 )
		{
			BlitImpl< copy4, 4 >( src, dest, x, y );
		}else if( src.colorChannel == 3 )
		{
			BlitImpl< copy3, 3 >( src, dest, x, y );
		}
	}
}