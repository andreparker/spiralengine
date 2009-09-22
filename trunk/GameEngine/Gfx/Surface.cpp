#include "Surface.hpp"

#include <memory.h>
#include <boost/assert.hpp>
#include <boost/make_shared.hpp>
#include "GfxDriver.hpp"
#include "Texture.hpp"
#include "GfxImpl.hpp"

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

Surface::~Surface()
{}

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
		memset( m_data.get(), value , m_width * m_height * m_elementSize );
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

		driver->CreateTexture( info, texture, m_data.get() );
	}

	return texture;
}