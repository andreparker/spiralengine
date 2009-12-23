#include "Texture.hpp"
#include "../Core/Log.hpp"

using namespace Spiral;

Texture::Texture():
m_width(0),m_height(0),
m_bitDepth(0),m_resource()
{

}

Texture::Texture( boost::int32_t width, boost::int32_t height, boost::int32_t bitDepth ):
m_width( width ),m_height( height ),
m_bitDepth( bitDepth ),m_resource()
{

}

Texture::~Texture()
{
	LOG_D( "^rTexture::~Texture - ^wwidth ^g%1% ^wheight ^g%2% ^wbitDepth ^g%3% ^rDestroyed\n", m_width, m_height, m_bitDepth );
}

Cloneable* Texture::DoClone()const
{
	return NULL;	
}
