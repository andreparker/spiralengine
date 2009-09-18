#include "OglSWVertexBufferResource.hpp"
#include "../../Resource/ResLockImpl.hpp"

using namespace Spiral;
using namespace boost;
using namespace std;

OglSWVertexBufferResource::OglSWVertexBufferResource( boost::int32_t size ):
m_isValid( true ),
m_data( NULL ),
m_size( size ),
m_mutex()
{
	m_data = new int8_t[ size ];
}


OglSWVertexBufferResource::~OglSWVertexBufferResource()
{
	delete [] m_data;
}

bool OglSWVertexBufferResource::DoIsValid() const
{
	return m_isValid;
}

bool OglSWVertexBufferResource::DoLock( int32_t start, int32_t size, ResLockInfo_t& info, bool bDiscard )
{
	int32_t totalSize = start + size;
	bool lockResults = false;

	if( m_mutex.try_lock() )
	{
		if( totalSize <= m_size )
		{
			info.data = (m_data + start);
			info.size = size;
			lockResults = true;
		}
	}
	
	return lockResults;
}

void OglSWVertexBufferResource::DoUnlock()
{
	m_mutex.unlock();
}

// cannot do a rectangle lock on a vertexbuffer
bool OglSWVertexBufferResource::DoLock(  ResLockRtInfo_t& /*info*/, bool /*bDiscard*/ )
{
	return false;
}

int32_t OglSWVertexBufferResource::DoSize() const
{
	return m_size;
}

void OglSWVertexBufferResource::DoSize( Rect< int32_t >& /*rect*/ ) const
{
}