#ifndef IMAGE_HPP
#define IMAGE_HPP

#include <boost/cstdint.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/scoped_array.hpp>

namespace Spiral
{

	class IFile;
namespace GfxUtil
{
	struct Image
	{
		boost::int32_t width,height;
		boost::int32_t bitDepth;			///< 16,24,32 bit depths
		boost::scoped_array< boost::int8_t > data;

		Image( boost::int32_t width_, boost::int32_t height_, boost::int32_t bitDepth_, boost::int8_t* data_ ):
		width( width_ ), height( height_ ), bitDepth( bitDepth_ ), data( data_ ){}


		struct ImageDesc
		{
			boost::int8_t* data;
			boost::int32_t width,height;
			boost::int32_t rowBytes;
			boost::int32_t colorChannel;
		};

		static void Blit( const ImageDesc& src, ImageDesc& dest, boost::int32_t x, boost::int32_t y );

		/*!
		   @function  LoadPng
		   @brief     loads a png file and returns a shared image pointer
		   @return    boost::shared_ptr< Image >
		   @param     boost::shared_ptr<IFile> & pngFile
		*/
		static boost::shared_ptr< Image > LoadPng( boost::shared_ptr<IFile>& pngFile );

	protected:
		Image(): width( 0 ), height( 0 ), bitDepth( 0 ), data( NULL ){} 
	};
}

}
#endif