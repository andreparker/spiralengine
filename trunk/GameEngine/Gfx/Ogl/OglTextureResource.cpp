#include "../../../ThirdParty/GLee/GLee.h"
#include "OglTextureResource.hpp"
#include "../GfxImpl.hpp"
#include "../../Resource/ResLockImpl.hpp"

using namespace boost;
using namespace Spiral;

OglTextureResource::OglTextureResource( const TextureInfo_t& info, const boost::int8_t* data ):
m_oglTextureId( 0 ),m_colorChannels(0), m_isValid( false ),m_rect(),m_data(NULL),m_mutex()
{
	GLuint textureId;
	glGenTextures( 1, &textureId );

	if( textureId )
	{
		m_isValid = true; 
		m_oglTextureId = static_cast<uint32_t>( textureId );
		glBindTexture( GL_TEXTURE_2D, textureId );

		GLint pixelType = ( info.bitDepth == 32 ? GL_RGBA : GL_RGB);
		m_colorChannels = info.bitDepth / 8;
		glTexImage2D( GL_TEXTURE_2D, 0, pixelType, info.width, info.height, 0, pixelType, GL_UNSIGNED_BYTE, data );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );

		m_rect = Rect< int32_t >( 0, info.width, info.height, 0 );
	}
}

OglTextureResource::~OglTextureResource()
{
	if( m_oglTextureId )
	{
		glDeleteTextures( 1, reinterpret_cast<const GLuint*>(&m_oglTextureId) );
	}
}

bool OglTextureResource::DoIsValid() const
{
	return m_isValid;
}

bool OglTextureResource::DoLock( boost::int32_t /*start*/, boost::int32_t /*size*/, ResLockInfo_t& /*info*/, bool /*bDiscard*/ )
{
	return false;
}

bool OglTextureResource::DoLock(  ResLockRtInfo_t& info, bool bDiscard )
{
	if( m_mutex.try_lock() )
	{
		m_data.reset( new int8_t[ m_rect.right * m_rect.top * m_colorChannels ] );
		info.rowBytes =  m_rect.right * m_colorChannels;
		info.rect = m_rect;
		info.data = m_data.get();

		glBindTexture( GL_TEXTURE_2D, static_cast<GLuint>(m_oglTextureId) );

		if( bDiscard )
		{
			GLint format = ( m_colorChannels == 4 ? GL_RGBA : GL_RGB);
			glTexImage2D( GL_TEXTURE_2D, 0, format, m_rect.right, m_rect.bottom, 0, format, GL_UNSIGNED_BYTE, NULL );
		}

		glGetTexImage( GL_TEXTURE_2D, 0, ( m_colorChannels == 4 ? GL_RGBA : GL_RGB), GL_UNSIGNED_BYTE, m_data.get() );

		return true;
	}

	return false;
}

void OglTextureResource::DoUnlock()
{
	GLint format = ( m_colorChannels == 4 ? GL_RGBA : GL_RGB);
	glBindTexture( GL_TEXTURE_2D, static_cast<GLuint>(m_oglTextureId) );
	glTexImage2D( GL_TEXTURE_2D, 0, format, m_rect.right, m_rect.bottom, 0, format, GL_UNSIGNED_BYTE, m_data.get() );

	m_data.reset();
	m_mutex.unlock();
}

boost::int32_t OglTextureResource::DoSize() const
{
	return 0;
}

void OglTextureResource::DoSize( Rect< boost::int32_t >& rect ) const
{
	rect = m_rect;
}