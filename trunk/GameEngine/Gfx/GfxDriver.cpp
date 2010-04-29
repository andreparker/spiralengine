#include "GfxDriver.hpp"
#include "GfxImpl.hpp"
#include "Texture.hpp"
#include "GfxDeviceCapability.hpp"
#include "../Common/BitManipulation.hpp"
#include "Utility/Image.hpp"
#include <boost/scoped_array.hpp>
#include <memory.h>

using namespace Spiral;
using namespace boost;

bool GfxDriver::CreateTexture(const TextureInfo_t &info, shared_ptr<Texture> &texture, const int8_t *data /*= 0*/)
{
	bool copyImage = false;
	int32_t newWidth = info.width,
		newHeight = info.height;
	const int8_t* finalData = data;
	TextureInfo_t newInfo;
	GfxDeviceCaps caps;
	scoped_array< int8_t > newImageData;

	GetCaps( caps );
	if( caps.nonPowerOf2Texture == false )
	{
		if( !PowerOf2( info.width ) )
		{
			newWidth = NextPowerOf2( info.width );
			copyImage = true;
		}

		if( !PowerOf2( info.height ) )
		{
			newHeight = NextPowerOf2( info.height );
			copyImage = true;
		}
	}
	
	if( copyImage && data )
	{
		// set up for transfer to a power of 2 surface
		GfxUtil::Image::ImageDesc src,dst;

		dst.colorChannel = info.bitDepth / 8;
		dst.width = newWidth;
		dst.height = newHeight;
		dst.rowBytes = dst.width * dst.colorChannel;

		const int32_t imageSize = newWidth * newHeight * dst.colorChannel;
		newImageData.reset( new int8_t[ imageSize ] );
		memset( newImageData.get(), 0, imageSize );

		dst.data = newImageData.get();

		src.colorChannel = dst.colorChannel;
		src.width = info.width;
		src.height = info.height;
		src.rowBytes = info.width * src.colorChannel;
		src.data = const_cast< int8_t* >( data );

		GfxUtil::Image::Blit( src, dst, 0, 0 );

		// update our info
		newInfo.width = newWidth;
		newInfo.height = newHeight;
		newInfo.bitDepth = info.bitDepth;
		newInfo.bManaged = info.bManaged;
		finalData = newImageData.get();

	}else
	{
		newInfo.width = newWidth;
		newInfo.height = newHeight;
		newInfo.bitDepth = info.bitDepth;
		newInfo.bManaged = info.bManaged;
	}

	return DoCreateTexture( newInfo, texture, finalData );
	
}

bool GfxDriver::SetVideo( const GfxVidInfo_t& info, bool bfullscreen /*= true*/ )
{
	return DoSetVideo( info, bfullscreen );
}

bool GfxDriver::DoEnumerateSettings( std::list< GfxVidInfo_t >& modeList )
{
	return DoEnumerateSettings( modeList );
}

bool GfxDriver::BeginDraw()
{
	return DoBeginDraw();
}

void GfxDriver::EndDraw()
{
	DoEndDraw();
}

bool GfxDriver::Present()
{
	return DoPresent();
}

void GfxDriver::Bind( const shared_ptr< Texture >& texture, int32_t unit /*= 0*/ )
{
	DoBind( texture, unit );
}

bool GfxDriver::CreateGeometry( const GeometryType& type, boost::shared_ptr<Geometry>& geometry )
{
	return DoCreateGeometry( type, geometry );
}

void GfxDriver::Draw( boost::shared_ptr<Geometry>& geometry )
{
	DoDraw( geometry );
}

void GfxDriver::SetState( const RenderState& state )
{
	DoSetState( state );
}

void GfxDriver::SetViewPort( boost::int32_t x0, boost::int32_t y0, boost::int32_t x1, boost::int32_t y1 )
{
	DoSetViewPort( x0, y0, x1, y1 );
}

void GfxDriver::ClearBuffer( const BufferInfo_t& buffer )
{
	DoClearBuffer(buffer);
}

void GfxDriver::Set( const ClearInfoType_t& type, boost::int32_t value )
{
	DoSet( type, value );
}

void GfxDriver::Set( const ClearInfoType_t& type, const Rgba& color )
{
	DoSet( type, color );
}

void GfxDriver::Set( const ClearInfoType_t& type, SpReal value )
{
	DoSet( type, value );
}

bool GfxDriver::CreateSprite( boost::shared_ptr< Sprite >& sprite, boost::shared_ptr< Texture >& texture, const Rect< SpReal >& spriteTexCoords, const Rect< SpReal >& spriteInfo )
{
	return DoCreateSprite( sprite, texture, spriteTexCoords, spriteInfo );
}

void GfxDriver::Draw( boost::shared_ptr<Sprite>& sprite )
{
	DoDraw( sprite );
}

void GfxDriver::SetWorld( const Math::Matrix4x4f& world )
{
	DoSetWorld( world );
}

void GfxDriver::SetView( const Math::Matrix4x4f& view )
{
	DoSetView( view );
}

void GfxDriver::SetProjection( const Math::Matrix4x4f& proj )
{
	DoSetProjection( proj );
}

void GfxDriver::GetView( Math::Matrix4x4f& view )
{
	DoGetView( view );
}

void GfxDriver::GetProjection( Math::Matrix4x4f& proj )
{
	DoGetProjection( proj );
}

void GfxDriver::Draw( const std::list< Sprite* >& spriteList )
{
	DoDraw( spriteList );
}

void GfxDriver::Set( const BlendMode_t& mode )
{
	DoSet( mode );
}

void GfxDriver::Draw( const Math::Vector2f& position, const Rect< SpReal >& rect, const Rect< SpReal >& textureCoords )
{
	DoDraw( position, rect, textureCoords );
}

void GfxDriver::GetViewPort( Rect<boost::int32_t>& viewPort )
{
	DoGetViewPort( viewPort );
}

void GfxDriver::SetClipRect( boost::int32_t x0, boost::int32_t y0, boost::int32_t x1, boost::int32_t y1 )
{
	DoSetClipRect( x0, y0, x1, y1 );
}

void GfxDriver::GetCaps( GfxDeviceCaps& caps ) const
{
	DoGetCaps( caps );
}