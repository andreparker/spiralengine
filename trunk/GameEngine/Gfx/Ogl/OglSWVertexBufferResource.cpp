#include "OglSWVertexBufferResource.hpp"

using namespace Spiral;
using namespace boost;
using namespace std;

OglSWVertexBufferResource::OglSWVertexBufferResource( boost::int32_t size ):
m_isValid( false )
m_data( NULL ),
m_size( size )
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
	if( totalSize < m_size )
	{
		info.data = (m_data + start);
		info.size = size;
	}
}