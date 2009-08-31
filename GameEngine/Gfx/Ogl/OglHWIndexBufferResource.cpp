#include "../../../ThirdParty/GLee/GLee.h"
#include "../../Resource/ResLockImpl.hpp"
#include "OglHWIndexBufferResource.hpp"


using namespace Spiral;
using namespace boost;

OglHWIndexBufferResource::OglHWIndexBufferResource( int32_t size ):
m_isValid( false ),
m_bufferId( 0 ),
m_size( size ),
m_mutex()
{
	if( GLEE_ARB_vertex_buffer_object )
	{
		glGenBuffersARB( 1, reinterpret_cast<GLuint*>(&m_bufferId) );
		glBindBufferARB( GL_ELEMENT_ARRAY_BUFFER_ARB, m_bufferId );
		glBufferDataARB( GL_ELEMENT_ARRAY_BUFFER_ARB, static_cast<GLsizei>(size), NULL, GL_DYNAMIC_DRAW_ARB );
	}

	if( m_bufferId != 0 )
	{
		m_isValid = true;
	}
}

void OglHWIndexBufferResource::Bind()
{
	glBindBufferARB( GL_ELEMENT_ARRAY_BUFFER_ARB, m_bufferId );
}

void OglHWIndexBufferResource::UnBind()
{
	glBindBufferARB( GL_ELEMENT_ARRAY_BUFFER_ARB, 0 );
}

bool OglHWIndexBufferResource::DoIsValid() const
{
	return m_isValid;
}

bool OglHWIndexBufferResource::DoLock( boost::int32_t start, boost::int32_t size, ResLockInfo_t& info, bool bDiscard )
{
	int8_t* data = NULL;
	if( m_bufferId && (start + size) < m_size )
	{
		Bind();
		if( bDiscard )
		{
			glBufferDataARB( GL_ELEMENT_ARRAY_BUFFER_ARB, static_cast<GLsizei>(m_size), NULL, GL_DYNAMIC_DRAW_ARB );
		}
		int8_t* data = reinterpret_cast<int8_t*>( glMapBufferARB( GL_ELEMENT_ARRAY_BUFFER_ARB, GL_READ_WRITE_ARB ) );
		info.data = start + data;
		info.size = start + size;
		m_isValid = true;

		if( data == NULL )
		{
			m_isValid = false;
		}
	}

	return ( m_isValid && data );
}

bool OglHWIndexBufferResource::DoLock( const Rect< int32_t >& /*rect*/, ResLockRtInfo_t& /*info*/, bool /*bDiscard*/ )
{
	return false;
}

void OglHWIndexBufferResource::DoUnlock()
{
	if( m_isValid )
	{
		GLboolean result = glUnmapBufferARB( GL_ELEMENT_ARRAY_BUFFER_ARB );
		if( result == GL_FALSE )
		{
			m_isValid = false;
		}
	}
	UnBind();
}

int32_t OglHWIndexBufferResource::DoSize() const
{
	return m_size;
}

void OglHWIndexBufferResource::DoSize( Rect< int32_t >& /*rect*/ ) const
{

}

OglHWIndexBufferResource::~OglHWIndexBufferResource()
{
	if( m_bufferId )
	{
		glDeleteBuffersARB( 1, reinterpret_cast<GLuint*>(&m_bufferId) );
	}
}