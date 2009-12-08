#include "Surface.hpp"

#include <memory.h>
#include <boost/assert.hpp>
#include <boost/make_shared.hpp>
#include "GfxDriver.hpp"
#include "Texture.hpp"
#include "GfxImpl.hpp"
#include "Color.hpp"

using namespace Spiral;
using namespace boost;

Surface::Surface( boost::int32_t width, boost::int32_t height, boost::int32_t elementSize ):
m_width( width ),m_height( height ),m_elementSize( elementSize ),m_data()
{
	BOOST_ASSERT( width > 0 && height > 0 && elementSize > 0 );
	int32_t bufferSize = width * height * elementSize;
	m_data.reset( new boost::int8_t[ bufferSize ] );

	Clear();
}

Surface::Surface( boost::int32_t width, boost::int32_t height, boost::int32_t elementSize, boost::int32_t /*dummy*/ ):
m_width( width ),m_height( height ),m_elementSize( elementSize ),m_data()
{
	BOOST_ASSERT( width > 0 && height > 0 && elementSize > 0 );
}

Surface::~Surface()
{

}

boost::shared_ptr< Surface > Surface::CreateRgb24bitSurface( boost::int32_t width, boost::int32_t height )
{
	return make_surface( width, height, 3 );
}

boost::shared_ptr< Surface > Surface::CreateRgba32bitSurface( boost::int32_t width, boost::int32_t height )
{
	return make_surface( width, height, 4 );
}

void Surface::Clear( boost::int32_t value /*=0*/)
{
	if( m_data )
	{
		memset( GetData() , value , m_width * m_height * m_elementSize );
	}
}

boost::shared_ptr< Texture > Surface::CreateTextureFromData( const boost::shared_ptr< GfxDriver >& driver )
{
	shared_ptr< Texture > texture;
	
	if( driver )
	{
		TextureInfo_t info;

		info.bitDepth = m_elementSize * 8;
		info.bManaged = false;
		info.width = m_width;
		info.height = m_height;

		driver->CreateTexture( info, texture, GetData() );
	}

	return texture;
}

void Surface::Fill( const Rgba& color )
{
	if( m_elementSize == 3 )
	{
		Fill_24( color );
	}else if( m_elementSize == 4 )
	{
		Fill_32( color );
	}
}

inline void StoreRgb( unsigned char* ptr, const Rgba& color )
{
	ptr[0] = color.GetByteComponent( 0 );
	ptr[1] = color.GetByteComponent( 1 );
	ptr[2] = color.GetByteComponent( 2 );
}

inline void StoreRgba( unsigned char* ptr, const Rgba& color )
{
	StoreRgb( ptr, color );
	ptr[3] = color.GetByteComponent( 3 );
}

template< void (*store)( unsigned char*,const Rgba& ) >
void ColorFill( unsigned char* ptr, int32_t count, int32_t stride, const Rgba& color )
{
	while( count > 0 )
	{
		store( ptr, color );
		ptr += stride;
		--count;
	}
}

void Surface::Fill_24( const Rgba& color )
{
	ColorFill< StoreRgb >( reinterpret_cast<unsigned char*>(GetData()), m_width * m_height, m_elementSize, color );
}

void Surface::Fill_32( const Rgba& color )
{
	ColorFill< StoreRgba >( reinterpret_cast<unsigned char*>(GetData()), m_width * m_height, m_elementSize, color );
}
