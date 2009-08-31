#include "OglHWVertexBufferResource.hpp"
#include "../../../ThirdParty/GLee/GLee.h"
#include "../../Resource/ResLockImpl.hpp"
#include "OglUtility.hpp"

using namespace Spiral;
using namespace boost;

OglHWVertexBufferResource::OglHWVertexBufferResource( boost::int32_t size ):
m_isValid(false),
m_bufferId(0),
m_size(size),
m_mutex()
{
	if( GLEE_ARB_vertex_buffer_object )
	{
		glGenBuffersARB( 1, reinterpret_cast<GLuint*>(&m_bufferId) );
		glBindBufferARB( GL_ARRAY_BUFFER_ARB, m_bufferId );
		glBufferDataARB( GL_ARRAY_BUFFER_ARB,static_cast<GLsizei>(size), NULL, GL_DYNAMIC_DRAW_ARB );
		glBindBufferARB( GL_ARRAY_BUFFER_ARB, 0 ); // unbind
	}
	
	if( m_bufferId != 0 )
	{
		m_isValid = true;
	}
}

void OglHWVertexBufferResource::Bind()
{
	glBindBufferARB( GL_ARRAY_BUFFER_ARB, m_bufferId );
}

void OglHWVertexBufferResource::UnBind()
{
	glBindBufferARB( GL_ARRAY_BUFFER_ARB, 0 );
}

OglHWVertexBufferResource::~OglHWVertexBufferResource()
{
	if( m_bufferId )
	{
		glDeleteBuffersARB( 1, reinterpret_cast<GLuint*>(&m_bufferId) );
	}
}

bool OglHWVertexBufferResource::DoIsValid() const
{
	return m_isValid;
}

bool OglHWVertexBufferResource::DoLock( int32_t start, int32_t size, ResLockInfo_t& info, bool bDiscard )
{
	if( m_bufferId )
	{
		Bind();
		if( bDiscard )
		{
			// discard the previous data
			glBufferDataARB( GL_ARRAY_BUFFER_ARB, static_cast<GLsizei>(start+size), NULL, GL_DYNAMIC_DRAW_ARB );
		}
		int8_t* data = reinterpret_cast<int8_t*>( glMapBufferARB( GL_ARRAY_BUFFER_ARB, GL_READ_WRITE_ARB ) );
		info.data = start + data;
		info.size = start + size;
		m_isValid = true;
		if( data == NULL )
		{
			m_isValid = false;
		}
	}

	return ( m_isValid );
}

bool OglHWVertexBufferResource::DoLock( const Rect< boost::int32_t >& /*rect*/, ResLockRtInfo_t& /*info*/, bool /*bDiscard*/ )
{
	return false;
}

void OglHWVertexBufferResource::DoUnlock()
{
	if( m_isValid )
	{
		GLboolean result = glUnmapBufferARB( GL_ARRAY_BUFFER_ARB );
		if( result == GL_FALSE )
		{
			m_isValid = false;
		}
	}
	UnBind();
}

int32_t OglHWVertexBufferResource::DoSize() const
{
	return m_size;
}

void OglHWVertexBufferResource::DoSize( Rect< int32_t >& /*rect*/ ) const
{
}