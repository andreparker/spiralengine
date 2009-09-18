#include <algorithm>
#include "OglSWIndexBufferResource.hpp"
#include "../../Resource/ResLockImpl.hpp"

using namespace Spiral;
using namespace boost;
using namespace std;

OglSWIndexBufferResource::OglSWIndexBufferResource( boost::int32_t size ):
m_isValid( true ),
m_data( NULL ),
m_size( size ),
m_mutex()
{
	m_data = new int8_t[size];
	fill( m_data, m_data + size, 0 );
}

OglSWIndexBufferResource::~OglSWIndexBufferResource()
{
	delete [] m_data;
}


bool OglSWIndexBufferResource::DoIsValid() const
{
	return m_isValid;
}

bool OglSWIndexBufferResource::DoLock( int32_t start, int32_t size, ResLockInfo_t& info, bool bDiscard )
{
	if( m_data && (start+size) < m_size )
	{
		if( m_mutex.try_lock() )
		{
			if( bDiscard )
			{
				fill( m_data, m_data + m_size, 0 );
			}

			info.data = m_data + start;
			info.size = start + size;
			return true;
		}
	}

	return false;
}

bool OglSWIndexBufferResource::DoLock( ResLockRtInfo_t& /*info*/, bool /*bDiscard*/ )
{
	return false;
}

void OglSWIndexBufferResource::DoUnlock()
{
	m_mutex.unlock();
}

int32_t OglSWIndexBufferResource::DoSize() const
{
	return m_size;
}

void Spiral::OglSWIndexBufferResource::DoSize( Rect< boost::int32_t >& /*rect*/ ) const
{

}